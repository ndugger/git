# pragma once

# include "git2/object.h"
# include "git2/oid.h"

# include "git/manager.cpp"

namespace git {

    class id {
        friend git::manager;

        private:
            git_oid* id_c_obj;

        protected:
            explicit id (git_oid* oid) : id_c_obj(oid) { }

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

            virtual const git_oid* c_id () {
                return git_object_id(object_c_obj);
            }

            virtual void* c_obj () {
                return &object_c_obj;
            }
    };
}