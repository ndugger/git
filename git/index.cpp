# pragma once

# include "git2/index.h"

# include "git/manager.cpp"
# include "git/object.cpp"

namespace git {

    class index : public git::object {
        friend git::manager;

        private:
            git_index* index_c_obj;

        protected:
            explicit index () : index_c_obj(nullptr) { }

            void* c_obj () {
                return &index_c_obj;
            }
    };
}