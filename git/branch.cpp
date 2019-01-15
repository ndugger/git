# pragma once

# include <string>

# include "git2/branch.h"
# include "git2/index.h"
# include "git2/repository.h"
# include "git2/refs.h"
# include "git2/tree.h"

# include "git/commit.cpp"
# include "git/manager.cpp"
# include "git/object.cpp"

namespace git {

class branch : protected git::object {
        friend git::manager;

        private:
            git_reference* branch_c_obj;
            std::string branch_name;
            bool branch_remote;

        protected:
            explicit branch (std::string name, bool remote = true) : branch_c_obj(nullptr), branch_name(name), branch_remote(remote) { }

            branch& lookup (git_repository** c_repository) {
                git_branch_lookup(&branch_c_obj, *c_repository, branch_name.c_str(), branch_remote ? GIT_BRANCH_REMOTE : GIT_BRANCH_LOCAL);
                return *this;
            }

            void* c_obj () override {
                return &branch_c_obj;
            }

        public:
            std::string& name () {
                return branch_name;
            }

            bool remote () {
                return branch_remote;
            }

            git::commit commit (const std::string& message) {

            }
    };
}