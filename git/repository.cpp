# pragma once

# include <array>
# include <iostream>
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

namespace {
    int git_repository_error = 0;
}

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
        bool exists () {
            return repository_c_obj != nullptr;
        }

        void set_head (git::branch& branch) {
            git_repository_error = git_repository_set_head(repository_c_obj, git_reference_name(*git::manager::c_obj<git::branch, git_reference**>(branch)));
        }

        void checkout_head (git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
            options.checkout_strategy = GIT_CHECKOUT_SAFE;

            git_repository_error = git_checkout_head(repository_c_obj, &options);
        }

        void checkout_index (git::index& index, git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
            options.checkout_strategy |= GIT_CHECKOUT_USE_THEIRS;

            git_repository_error = git_checkout_index(repository_c_obj, *git::manager::c_obj<git::index, git_index**>(index), &options);

        }

        git::tree checkout_tree (const std::string& name, git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
            options.checkout_strategy = GIT_CHECKOUT_SAFE;

            git::tree tree(git::manager::create<git::tree>());

            git_repository_error = git_revparse_single(git::manager::c_obj<git::tree, git_object**>(tree), repository_c_obj, name.c_str());
            git_repository_error = git_checkout_tree(repository_c_obj, *(git::manager::c_obj<git::tree, git_object**>(tree)), &options);

            return tree;
        }

        git::branch branch (const std::string& name) {
            return git::manager::lookup<git::branch>(
                    git::manager::create<git::branch>(name, false),
                    git::manager::c_obj<git::repository, git_repository**>(*this)
            );
        }

        git::remote remote (const std::string& name) {
            git_strarray names({ nullptr });

            git_repository_error = git_remote_list(&names, repository_c_obj);

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

            git_repository_error = git_merge(repository_c_obj, git::manager::c_obj<git::commit, const git_annotated_commit**>(commit_annotated), 1, &options, &checkout_options);

            git::index index(git::manager::lookup<git::index>(git::manager::create<git::index>(), &repository_c_obj));

            if (git_index_has_conflicts(*git::manager::c_obj<git::index, git_index**>(index))) {
                const git_index_entry* ancestor_out = nullptr;
                const git_index_entry* our_out = nullptr;
                const git_index_entry* their_out = nullptr;

                git_index_conflict_iterator* conflict_iterator = nullptr;
                git_index_conflict_iterator_new(&conflict_iterator, *git::manager::c_obj<git::index, git_index**>(index));

                while (git_index_conflict_next(&ancestor_out, &our_out, &their_out, conflict_iterator) != GIT_ITEROVER) {
                    if (ancestor_out) std::cout<< "ancestor: " << ancestor_out->path <<std::endl;
                    if (our_out) std::cout<< "our: " << our_out->path <<std::endl;
                    if (their_out) std::cout<< "their: " << their_out->path <<std::endl;
                }

                checkout_index(index);

                git_index_conflict_iterator_free(conflict_iterator);
            }

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

            git_oid new_tree_id;
            git_tree* new_tree = nullptr;

            git_repository_error = git_index_update_all(*git::manager::c_obj<git::index, git_index**>(index), nullptr, nullptr, nullptr);
            git_repository_error = git_index_write(*git::manager::c_obj<git::index, git_index**>(index));
            git_repository_error = git_index_write_tree(&new_tree_id, *git::manager::c_obj<git::index, git_index**>(index));
            git_repository_error = git_tree_lookup(&new_tree, repository_c_obj, &new_tree_id); // TODO This is a lookup

            git::id commit_id(git::manager::create<git::id>());
            git::signature signature(git::manager::create<git::signature>("fake user", "fake@email.address"));

            git_repository_error = git_commit_create_v(
                    git::manager::c_obj<git::id, git_oid*>(commit_id),
                    repository_c_obj,
                    git_reference_name(*git::manager::c_obj<git::branch, git_reference**>(branch_into)),
                    *git::manager::c_obj<git::signature, git_signature**>(signature),
                    *git::manager::c_obj<git::signature, git_signature**>(signature),
                    "UTF-8",
                    "fake commit",
                    new_tree,
                    2,
                    *git::manager::c_obj<git::commit, git_commit**>(commit_existing),
                    *git::manager::c_obj<git::commit, git_commit**>(commit_incoming)
            );

            return git::manager::lookup<git::commit>(
                    git::manager::create<git::commit>(),
                    &repository_c_obj,
                    git::manager::c_obj<git::branch, git_reference**>(branch_into)
            );
        }
    };
}