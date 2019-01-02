# pragma once

# include "git2.h"

namespace {
    void* git_manager_ptr;
}

namespace git {

    class manager {

        public:
            static void init () {
                git_manager_ptr = new manager;
            }

            static void clean_up () {
                delete static_cast<manager*>(git_manager_ptr);
            }

            template <typename object_type, typename... argument_types> static object_type create (argument_types... arguments) {
                return object_type(arguments...);
            }

            template <typename object_type, typename... argument_types> static object_type lookup (object_type object, argument_types... arguments) {
                return object.lookup(arguments...);
            }

            template <typename object_type> static git_oid** c_id (object_type& object) {
                return object.c_id();
            }

            template <typename object_type, typename c_object_type> static c_object_type c_obj (object_type& object) {
                return static_cast<c_object_type>(object.c_obj());
            }

        private:
            explicit manager () {
                git_libgit2_init();
            };

            ~ manager () {
                git_libgit2_shutdown();
            }
    };
}