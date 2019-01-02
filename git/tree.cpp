# pragma once

# include "git2/object.h"
# include "git2/tree.h"

# include "git/manager.cpp"
# include "git/object.cpp"

# include "git2/object.h"
# include "git2/repository.h"
# include "git2/tree.h"

namespace git {

    class tree : public git::object {
        friend git::manager;

        private:
            git_tree* tree_c_obj;
            git_object* tree_tree_ish;

        protected:
            explicit tree () : tree_c_obj(nullptr), tree_tree_ish(nullptr) { }

            void* c_obj () override {
                return &tree_c_obj;
            }
    };
}