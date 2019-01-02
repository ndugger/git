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
            git_commit* commit_c_obj;
            std::string commit_message;

        protected:
            explicit commit (const std::string& message = "") : commit_c_obj(nullptr) { }

            commit& lookup (git_repository** c_repository) {
                git_commit_lookup(&commit_c_obj, *c_repository, git::manager::c_obj<git::id, git_oid*>(id()));
                return *this;
            }

            void* c_obj () override {
                return &commit_c_obj;
            }

        public:
            std::string& message () {
                return commit_message;
            }
    };
}