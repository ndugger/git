# pragma once

# include <string>

# include "git2/clone.h"
# include "git2/repository.h"

# include "git/manager.cpp"
# include "git/options.cpp"
# include "git/repository.cpp"

namespace git {

    git::repository clone (const std::string& remote, const std::string& path, git_clone_options options = GIT_CLONE_OPTIONS_INIT) {
        git::repository repository(git::manager::create<git::repository>());
        git_repository* c_repository(git::manager::c_obj<git::repository, git_repository*>(repository));

        git_clone(&c_repository, remote.c_str(), path.c_str(), &options);

        return repository;
    }
}