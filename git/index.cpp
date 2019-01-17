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

            index& lookup (git_repository** c_repository) {
                git_repository_index(&index_c_obj, *c_repository);

                return *this;
            }

            void* c_obj () override {
                return &index_c_obj;
            }
    };
}