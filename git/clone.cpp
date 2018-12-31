# pragma once

# include <string>

# include "git2/clone.h"
# include "git/repository.cpp"

namespace git {

    git::repository clone (const std::string& remote, const std::string& path, git_clone_options options = GIT_CLONE_OPTIONS_INIT) {
        git::repository repository;

        git_clone(repository.c_ptr(), remote.c_str(), path.c_str(), &options);

        return repository;
    }
}