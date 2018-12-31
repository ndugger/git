# pragma once

# include <string>

# include "git2/repository.h"

namespace git {

    class repository {

        private:
            git_repository* repository_git_repository;

        public:
            explicit repository () {
                repository_git_repository = nullptr;
            }

            git_repository** c_ptr () {
                return &repository_git_repository;
            }
    };
}