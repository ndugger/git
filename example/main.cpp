# include <iostream>
# include <string>

# include "git/clone.cpp"
# include "git/open.cpp"
# include "git/remote.cpp"
# include "git/repository.cpp"
# include "git/util.cpp"

int main () {
    git::util::initialize();

    git::repository repository(git::clone("https://github.com/fyrware/git.git", "/home/nick/Projects/fyrware/git/ignore/git/"));

    for (git::remote& remote : repository.remotes()) {
        std::cout << remote.url() << std::endl;
    }

    git::util::uninitialize();
}