# pragma once

# include <array>
# include <iterator>
# include <map>
# include <string>
# include <vector>

# include "git2/checkout.h"
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
# include "git/options.cpp"
# include "git/remote.cpp"
# include "git/signature.cpp"
# include "git/tree.cpp"

namespace git {

    class repository {

        private:
            git_repository* repository_c_obj;
            std::string repository_path;

        public:
            explicit repository (const std::string& path) : repository_c_obj(nullptr), repository_path(path) { }

            git_repository** c_obj () {
                return &repository_c_obj;
            }

            std::string& path () {
                return repository_path;
            }

            git::index index () {

            }

            git::commit last_commit () {
                git::commit commit;

                git_commit_lookup(commit.c_obj(), repository_c_obj, *(commit.id()));

                return commit;
            }

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

            git::branch branch (const std::string& branch_name) {
                return git::branch(repository_c_obj, branch_name, false);
            }

            git::remote remote (const std::string& remote_name) {
                git_strarray names({ nullptr });

                git_remote_list(&names, repository_c_obj);

                for (const std::string& name : std::vector<std::string>(names.strings, names.strings + names.count)) {
                    if (remote_name == name) {
                        return git::remote(repository_c_obj, name);
                    }
                }
            }

            git::commit merge (git::branch& branch, git_merge_options options = GIT_MERGE_OPTIONS_INIT) {
                git_checkout_options checkout_options = GIT_CHECKOUT_OPTIONS_INIT;
                const git_annotated_commit* their_head[ 10 ];

                git_annotated_commit_from_ref((git_annotated_commit**)&their_head[ 0 ], repository_c_obj, branch.c_obj());
                git_merge(repository_c_obj, their_head, 1, &options, &checkout_options);

                git::commit commit("merge");
                git::index current_index(index());
                git::signature me("name", "email@address");
                git::tree new_tree;

                git_index_write_tree(new_tree.id(), current_index.c_obj());
                git_tree_lookup((git_tree**)new_tree.c_obj(), repository_c_obj, new_tree.id());
                git_commit_create_v(
                    (git_oid*)commit.id(),
                    repository_c_obj,
                    branch.name().c_str(),
                    me.c_obj(),
                    me.c_obj(),
                    "UTF-8",
                    commit.message().c_str(),
                    new_tree.c_obj(),
                    2,
                    commit.c_obj(),
                    last_commit().c_obj()
                );
            }
    };
}