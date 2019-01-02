# pragma once

# include "git2/object.h"
# include "git2/tree.h"

# include "git/manager.cpp"
# include "git/object.cpp"

namespace git {

    class tree : public git::object {
        friend git::manager;

        private:
            git_tree* tree_c_obj;
            git_oid* tree_id;
            git_object* tree_tree_ish;

        protected:
            explicit tree () : tree_c_obj(nullptr), tree_id(nullptr), tree_tree_ish(nullptr) { }

            void* c_obj () override {
                return &tree_c_obj;
            }

        public:
            git_object** tree_ish () {
                return static_cast<git_object**>(git::object::c_obj());
            }
    };
}