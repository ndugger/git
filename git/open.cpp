# pragma once

# include <string>

# include "git/repository.cpp"
# include "git2/repository.h"

namespace git {

    git::repository open (const std::string& path) {
        git::repository repository(path);

        git_repository_open(repository.c_ptr(), path.c_str());

        return repository;
    }
}