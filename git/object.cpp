# pragma once

# include "git2/object.h"
# include "git2/oid.h"

# include "git/manager.cpp"

namespace git {

    class id {
        friend git::manager;

        private:
            git_oid id_c_obj;

        protected:
            explicit id () : id_c_obj() { }

            void* c_obj () {
                return &id_c_obj;
            }
    };

    class object {
        friend git::manager;

        private:
            git_object* object_c_obj;

        protected:
            explicit object () : object_c_obj(nullptr) { }

            virtual void* c_obj () {
                return &object_c_obj;
            }
    };
}