# pragma once

# include <string>

# include "git2/repository.h"

# include "git/manager.cpp"
# include "git/repository.cpp"

namespace git {

    git::repository open (const std::string& path) {
        git::repository repository(git::manager::create<git::repository>());

        git_repository_open(git::manager::c_obj<git::repository, git_repository**>(repository), path.c_str());

        return repository;
    }
}