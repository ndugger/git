# pragma once

# include "git2/object.h"

namespace git {

    class object {

        private:
            git_object* object_c_obj;

        public:
            explicit object () : object_c_obj(nullptr) { }

            git_object** c_obj () {
                return &object_c_obj;
            }

            const git_oid* id () {
                return git_object_id(object_c_obj);
            }
    };
}