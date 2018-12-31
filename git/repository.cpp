# pragma once

# include <array>
# include <iterator>
# include <string>
# include <vector>

# include "git/remote.cpp"
# include "git2/remote.h"
# include "git2/repository.h"
# include "git2/strarray.h"

namespace git {

    class repository {

        private:
            git_repository* repository_c_obj;
            std::string repository_path;

        public:
            explicit repository (const std::string& path) {
                repository_c_obj = nullptr;
                repository_path = path;
            }

            git_repository** c_obj () {
                return &repository_c_obj;
            }

            std::string& path () {
                return repository_path;
            }

            std::vector<git::remote> remotes () {
                std::vector<git::remote> remotes;
                git_strarray names({ nullptr });

                git_remote_list(&names, repository_c_obj);

                remotes.reserve(names.count);

                for (const std::string& name : std::vector<std::string>(names.strings, names.strings + names.count)) {
                    git::remote remote(repository_c_obj);

                    git_remote_lookup(remote.c_obj(), repository_c_obj, name.c_str());

                    remotes.emplace_back(remote);
                }

                return remotes;
            }
    };
}