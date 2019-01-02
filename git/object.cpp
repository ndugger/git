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
            explicit id () : id_c_obj(nullptr) { }

            void* c_obj () {
                return &id_c_obj;
            }
    };

    class object {
        friend git::manager;

        private:
            git_object* object_c_obj;
            git::id object_id;

        protected:
            explicit object () : object_c_obj(nullptr), object_id(git::manager::create<git::id>()) { }

            virtual void* c_obj () {
                return &object_c_obj;
            }

        public:
            git::id& id () {
                return object_id;
            }
    };
}