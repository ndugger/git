# pragma once

# include <array>
# include <iterator>
# include <map>
# include <string>
# include <vector>

# include "git2/checkout.h"
# include "git2/index.h"
# include "git2/merge.h"
# include "git2/object.h"
# include "git2/oid.h"
# include "git2/refs.h"
# include "git2/remote.h"
# include "git2/repository.h"
# include "git2/revparse.h"
# include "git2/signature.h"
# include "git2/strarray.h"
# include "git2/tree.h"

# include "git/branch.cpp"
# include "git/commit.cpp"
# include "git/index.cpp"
# include "git/manager.cpp"
# include "git/object.cpp"
# include "git/options.cpp"
# include "git/remote.cpp"
# include "git/signature.cpp"
# include "git/tree.cpp"

namespace git {

    class repository : protected git::object {
        friend class git::manager;

        private:
            git_repository* repository_c_obj;

        protected:
            explicit repository () : repository_c_obj(nullptr) { }

            git_repository** c_obj () {
                return &repository_c_obj;
            }

        public:
            void set_head (git::branch branch) {
                git_repository_set_head(repository_c_obj, branch.name().c_str());
            }

            void checkout_head (git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_checkout_head(repository_c_obj, &options);
            }

            git::index checkout_index (git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
                git::index index;

                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_checkout_index(repository_c_obj, index.c_obj(), &options);

                return index;
            }

            git::tree checkout_tree (const std::string& name, git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
                git::tree tree;

                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_revparse_single(tree.tree_ish(), repository_c_obj, name.c_str());
                git_checkout_tree(repository_c_obj, *(tree.tree_ish()), &options);

                return tree;
            }

            git::branch branch (const std::string& name) {
                return git::manager::lookup<git::branch>(git::manager::create<git::branch>(name, false), &*repository_c_obj);
            }

            git::remote remote (const std::string& name) {
                git_strarray names({ nullptr });

                git_remote_list(&names, repository_c_obj);

                for (const std::string& string : std::vector<std::string>(names.strings, names.strings + names.count)) {
                    if (name == string) {
                        return git::manager::create<git::remote>(name);
                    }
                }
            }

            git::commit merge (git::branch& branch, git_merge_options options = GIT_MERGE_OPTIONS_INIT) {
                git_checkout_options checkout_options = GIT_CHECKOUT_OPTIONS_INIT;
                const git_annotated_commit* the_head[ 10 ];
                git_index* the_index = nullptr;

                git_annotated_commit_from_ref((git_annotated_commit**)&the_head[ 0 ], repository_c_obj, git::manager::c_obj<git::branch, git_reference*>(branch));
                git_merge(repository_c_obj, the_head, 1, &options, &checkout_options);

                git::commit commit_head(git::manager::lookup<git::commit>(git::manager::create<git::commit>(), repository_c_obj));
                git::commit commit_new(git::manager::create<git::commit>("merged"));
                git::signature signature_me(git::manager::create<git::signature>("name", "email@address"));
                git::tree tree_new(git::manager::create<git::tree>());

                git_repository_index(&the_index, repository_c_obj);
                git_checkout_index(repository_c_obj, the_index, &checkout_options);
                git_index_update_all(the_index, nullptr, nullptr, nullptr);
                git_index_write(the_index);
                git_index_write_tree(*(tree_new.id()), the_index);
                git_tree_lookup((git_tree**)tree_new.c_obj(), repository_c_obj, *(tree_new.id()));
                git_commit_create_v(
                    (git_oid*)commit_new.id(),
                    repository_c_obj,
                    branch.name().c_str(),
                    git::manager::c_obj<git::signature, git_signature*>(signature_me),
                    git::manager::c_obj<git::signature, git_signature*>(signature_me),
                    "UTF-8",
                    commit_new.message().c_str(),
                    git::manager::c_obj<git::tree, git_tree*>(tree_new),
                    2,
                    git::manager::c_obj<git::commit, git_commit*>(commit_new),
                    git::manager::c_obj<git::commit, git_commit*>(commit_head)
                );

                return commit_new;
            }
    };
}