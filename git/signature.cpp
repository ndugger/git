# pragma once

# include <string>

# include "git2/signature.h"

# include "git/manager.cpp"
# include "git/object.cpp"

namespace git {

    class signature : protected git::object {
        friend git::manager;

        private:
            git_signature* signature_c_obj;

        protected:
            explicit signature (const std::string& name, const std::string& email) : signature_c_obj(nullptr) {
                git_signature_now(&signature_c_obj, name.c_str(), email.c_str());
            }

            void* c_obj () override {
                return &signature_c_obj;
            }
    };
}