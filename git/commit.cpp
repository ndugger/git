# pragma once

# include <string>

# include "git2/branch.h"
# include "git2/commit.h"
# include "git2/oid.h"
# include "git2/refs.h"
# include "git2/repository.h"

namespace git {

    class commit {

        private:
            git_commit* commit_c_obj;
            const git_oid* commit_id;
            std::string commit_message;

        public:
            explicit commit (const std::string& message = "") : commit_c_obj(nullptr), commit_id(nullptr), commit_message(message) { }

            git_commit** c_obj () {
                return &commit_c_obj;
            }

            const git_oid** id () {
                return &commit_id;
            }

            std::string& message () {
                return commit_message;
            }
    };
}