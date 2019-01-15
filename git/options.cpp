# pragma once

# include <initializer_list>
# include <string>
# include <utility>
# include <variant>

# include "git2/checkout.h"

namespace {
    typedef git_options_type = std::initializer_list<std::pair<std::string, std::variant<bool, int, std::string>>>;
}

namespace git::options {

    class checkout_options {
        
        private:
            git_checkout_opts checkout_options_c_obj;
        
        protected:
            explicit checkout (git_options_type options) : checkout_options_c_obj(GIT_CHECKOUT_OPTIONS_INIT) {
                // TODO lots of logic to map options to checkout_options_c_obj
            }
    };
}
