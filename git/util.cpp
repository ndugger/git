# pragma once

# include "git2.h"

namespace git::util {

    void initialize () {
        git_libgit2_init();
    }

    void uninitialize () {
        git_libgit2_shutdown();
    }
}