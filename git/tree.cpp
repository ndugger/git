# pragma once

# include "git2/object.h"
# include "git2/tree.h"

namespace git {

    class tree {

        private:
            git_tree* tree_c_obj;
            git_oid* tree_id;
            git_object* tree_tree_ish;

        public:
            explicit tree () : tree_c_obj(nullptr), tree_tree_ish(nullptr) { }

            const git_tree* c_obj () {
                return tree_c_obj;
            }

            git_object** tree_ish () {
                return &tree_tree_ish;
            }

            git_oid* id () {
                return tree_id;
            }
    };
}