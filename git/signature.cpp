# pragma once

# include <string>

# include "git2/signature.h"

namespace git {

    class signature {

        private:
            git_signature* signature_c_obj;

        public:
            explicit signature (const std::string& name, const std::string& email) : signature_c_obj(nullptr) {
                git_signature_now(&signature_c_obj, name.c_str(), email.c_str());
            }

            const git_signature* c_obj () {
                return signature_c_obj;
            }
    };
}