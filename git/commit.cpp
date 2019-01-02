# pragma once

# include <string>

# include "git2/branch.h"
# include "git2/commit.h"
# include "git2/oid.h"
# include "git2/refs.h"
# include "git2/repository.h"

# include "git/manager.cpp"
# include "git/object.cpp"

namespace git {

class commit : public git::object {
        friend class git::manager;

        private:
            bool commit_annotated;
            git_commit* commit_c_obj;
            git_annotated_commit* commit_c_obj_annotated;
            std::string commit_message;

        protected:

            explicit commit (bool annotated = false) :
                commit_annotated(annotated),
                commit_c_obj(nullptr),
                commit_c_obj_annotated(nullptr),
                commit_message("") { }

            commit& lookup (git_repository** c_repository, git_reference** c_branch) {
                if (commit_annotated) {
                    git_annotated_commit_from_ref(&commit_c_obj_annotated, *c_repository, *c_branch);
                }
                else {
                    git_commit_lookup(&commit_c_obj, *c_repository, git_reference_target(*c_branch));
                }

                return *this;
            }

            const git_oid* c_id () override {
                return git_commit_id(commit_c_obj);
            }

            void* c_obj () override {
                if (commit_annotated) {
                    return &commit_c_obj_annotated;
                }
                else {
                    return &commit_c_obj;
                }
            }

        public:
            bool annotated () {
                return commit_annotated;
            }

            std::string& message () {
                return commit_message;
            }
    };
}