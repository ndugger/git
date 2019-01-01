# pragma once

# include "git2/index.h"

namespace git {

    class index {

        private:
            git_index* index_c_obj;

        public:
            explicit index () : index_c_obj(nullptr) { }

            git_index* c_obj () {
                return index_c_obj;
            }
    };
}