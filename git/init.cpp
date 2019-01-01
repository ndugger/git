# pragma once

# include <string>

# include "git/repository.cpp"
# include "git2/repository.h"

namespace git {

    git::repository init (const std::string& path) {
//        git::repository repository(path);
//
//        git_repository_init(repository.c_obj(), path.c_str(), 0);
//
//        return repository;
    }
}