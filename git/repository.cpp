# pragma once

# include <array>
# include <iterator>
# include <string>
# include <vector>

# include "git/index.cpp"
# include "git/object.cpp"
# include "git/remote.cpp"
# include "git2/checkout.h"
# include "git2/remote.h"
# include "git2/repository.h"
# include "git2/revparse.h"
# include "git2/strarray.h"

namespace git {

    class repository {

        private:
            git_repository* repository_c_obj;
            std::string repository_path;

        public:
            explicit repository (const std::string& path) : repository_c_obj(nullptr), repository_path(path) { }

            git_repository** c_obj () {
                return &repository_c_obj;
            }

            std::string& path () {
                return repository_path;
            }

            std::vector<git::remote> remotes () {
                std::vector<git::remote> remotes;
                git_strarray names({ nullptr });

                git_remote_list(&names, repository_c_obj);
                remotes.reserve(names.count);

                for (const std::string& name : std::vector<std::string>(names.strings, names.strings + names.count)) {
                    git::remote remote(repository_c_obj);

                    git_remote_lookup(remote.c_obj(), repository_c_obj, name.c_str()); // this probably doesn't belong here
                    remotes.emplace_back(remote);
                }

                return remotes;
            }

            void checkout_head (git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_checkout_head(repository_c_obj, &options);
            }

            git::index checkout_index (git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
                git::index index;

                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_checkout_index(repository_c_obj, index.c_obj(), &options);

                return index;
            }

            git::object checkout_tree (const std::string& tree, git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT) {
                git::object object;

                options.checkout_strategy = GIT_CHECKOUT_SAFE;
                git_revparse_single(object.c_obj(), repository_c_obj, tree.c_str());
                git_checkout_tree(repository_c_obj, *(object.c_obj()), &options);

                return object;
            }
    };
}