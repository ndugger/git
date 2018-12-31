# pragma once

# include <string>

# include "git/repository.cpp"
# include "git2/clone.h"

namespace git {

    git::repository clone (const std::string& remote, const std::string& path, git_clone_options options = GIT_CLONE_OPTIONS_INIT) {
        git::repository repository(path);

        git_clone(repository.c_obj(), remote.c_str(), path.c_str(), &options);

        return repository;
    }
}