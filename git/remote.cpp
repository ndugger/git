# pragma once

# include <string>

# include "git/branch.cpp"
# include "git2/remote.h"
# include "git2/repository.h"

namespace git {

    class remote {

        private:
            git_remote* remote_c_obj;
            git_repository* remote_c_repository_obj;

        public:
            explicit remote (git_repository* repository, const std::string& name) : remote_c_obj(nullptr), remote_c_repository_obj(repository) {
                git_remote_lookup(&remote_c_obj, repository, name.c_str());
            }

            git_remote** c_obj () {
                return &remote_c_obj;
            }

            std::string name () {
                return git_remote_name(remote_c_obj);
            }

            std::string url () {
                return git_remote_url(remote_c_obj);
            }

            std::string push_url () {
                return git_remote_pushurl(remote_c_obj);
            }

            git::branch branch (const std::string& branch_name) {
                return git::branch(remote_c_repository_obj, (name() + "/" + branch_name));
            }

            void fetch () {
                git_remote_fetch(remote_c_obj, nullptr, nullptr, nullptr);
            }
    };
}