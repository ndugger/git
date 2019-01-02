# pragma once

# include <string>

# include "git2/remote.h"
# include "git2/repository.h"

# include "git/branch.cpp"
# include "git/manager.cpp"
# include "git/object.cpp"

namespace git {

class remote : protected git::object {
        friend git::manager;

        private:
            git_remote* remote_c_obj;
            std::string remote_name;

        protected:
            explicit remote (const std::string& name = "") : remote_c_obj(nullptr), remote_name(name) { }

            remote& lookup (git_repository** c_repository) {
                git_remote_lookup(&remote_c_obj, *c_repository, remote_name.c_str());
                return *this;
            }

            void* c_obj () override {
                return &remote_c_obj;
            }

        public:
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
                git_repository* repo(git_remote_owner(remote_c_obj));
                return git::manager::lookup<git::branch>(git::manager::create<git::branch>(name() + "/" + branch_name), &repo);
            }

            void fetch () {
                git_remote_fetch(remote_c_obj, nullptr, nullptr, nullptr);
            }
    };
}