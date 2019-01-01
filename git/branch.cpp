# pragma once

# include <string>

# include "git/commit.cpp"
# include "git2/branch.h"
# include "git2/index.h"
# include "git2/repository.h"
# include "git2/refs.h"
# include "git2/tree.h"

namespace git {

    class branch {

        private:
            git_reference* branch_c_obj;
            git_repository* branch_c_repository_obj;
            std::string branch_name;
            bool branch_remote;

        public:
            explicit branch (git_repository* repository, const std::string& name, bool remote = true) : branch_c_obj(nullptr), branch_c_repository_obj(repository), branch_name(name), branch_remote(remote) {
                git_branch_lookup(&branch_c_obj, repository, branch_name.c_str(), remote ? GIT_BRANCH_REMOTE : GIT_BRANCH_LOCAL);
            }

            const git_reference* c_obj () {
                return branch_c_obj;
            }

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