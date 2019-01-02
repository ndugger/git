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

            void* c_obj () override {
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
                git::index index(git::manager::create<git::index>());

                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_checkout_index(repository_c_obj, *git::manager::c_obj<git::index, git_index**>(index), &options);

                return index;
            }

//            git::tree checkout_tree (const std::string& name, git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
//                git::tree tree(git::manager::create<git::tree>());
//
//                options.checkout_strategy = GIT_CHECKOUT_SAFE;
//                git_revparse_single(tree.tree_ish(), repository_c_obj, name.c_str());
//                git_checkout_tree(repository_c_obj, *(tree.tree_ish()), &options);
//
//                return tree;
//            }

            git::branch branch (const std::string& name) {
                return git::manager::lookup<git::branch>(
                    git::manager::create<git::branch>(name, false),
                    git::manager::c_obj<git::repository, git_repository**>(*this)
                );
            }

            git::remote remote (const std::string& name) {
                git_strarray names({ nullptr });

                git_remote_list(&names, repository_c_obj);

                for (const std::string& string : std::vector<std::string>(names.strings, names.strings + names.count)) {
                    if (name == string) {
                        return git::manager::lookup<git::remote>(
                            git::manager::create<git::remote>(name),
                            git::manager::c_obj<git::repository, git_repository**>(*this)
                        );
                    }
                }
            }

            git::commit merge (git::branch& branch_into, git::branch& branch_from, git_merge_options options = GIT_MERGE_OPTIONS_INIT) {
                git_checkout_options checkout_options = GIT_CHECKOUT_OPTIONS_INIT;

                git::commit commit_annotated(git::manager::lookup<git::commit>(
                    git::manager::create<git::commit>(true),
                    &repository_c_obj,
                    git::manager::c_obj<git::branch, git_reference**>(branch_from)
                ));

                git_merge(repository_c_obj, git::manager::c_obj<git::commit, const git_annotated_commit**>(commit_annotated), 1, &options, &checkout_options);

                git::index index(git::manager::create<git::index>());
                git::tree tree(git::manager::create<git::tree>());

                git_repository_index(git::manager::c_obj<git::index, git_index**>(index), repository_c_obj);
                git_checkout_index(repository_c_obj, *git::manager::c_obj<git::index, git_index**>(index), &checkout_options);
                git_revparse_single((git_object**)git::manager::c_obj<git::tree, git_tree**>(tree), repository_c_obj, "HEAD~^{tree}");

                git::commit commit_existing(git::manager::lookup<git::commit>(
                        git::manager::create<git::commit>(),
                        &repository_c_obj,
                        git::manager::c_obj<git::branch, git_reference**>(branch_into)
                ));

                git::commit commit_incoming(git::manager::lookup<git::commit>(
                        git::manager::create<git::commit>(),
                        &repository_c_obj,
                        git::manager::c_obj<git::branch, git_reference**>(branch_from)
                ));

                git_index_update_all(*git::manager::c_obj<git::index, git_index**>(index), nullptr, nullptr, nullptr);
                git_index_write(*git::manager::c_obj<git::index, git_index**>(index));
                git_index_write_tree(const_cast<git_oid*>(git_tree_id(*git::manager::c_obj<git::tree, git_tree**>(tree))), *git::manager::c_obj<git::index, git_index**>(index));
                git_tree_lookup(git::manager::c_obj<git::tree, git_tree**>(tree), repository_c_obj, const_cast<git_oid*>(git_tree_id(*git::manager::c_obj<git::tree, git_tree**>(tree))));

                git::commit commit_new(git::manager::create<git::commit>());
                git::signature signature(git::manager::create<git::signature>("name", "email@address"));

                git_commit_create_v(
                    *git::manager::c_id<git::commit>(commit_new),
                    repository_c_obj,
                    branch_into.name().c_str(),
                    *git::manager::c_obj<git::signature, git_signature**>(signature),
                    *git::manager::c_obj<git::signature, git_signature**>(signature),
                    nullptr,
                    "branches merged",
                    *git::manager::c_obj<git::tree, git_tree**>(tree),
                    2,
                    *git::manager::c_obj<git::commit, git_commit**>(commit_incoming),
                    *git::manager::c_obj<git::commit, git_commit**>(commit_existing)
                );

                return commit_new;
            }
    };
}