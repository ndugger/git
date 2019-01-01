# include <iostream>
# include <string>

# include "git/clone.cpp"
# include "git/init.cpp"
# include "git/object.cpp"
# include "git/open.cpp"
# include "git/remote.cpp"
# include "git/repository.cpp"
# include "git/util.cpp"

int main () {
    git::util::initialize();

    git::repository cloned(git::clone("https://github.com/fyrware/git.git", "/home/nick/Projects/fyrware/git/ignore/git/"));

    for (git::remote& remote : cloned.remotes()) {
        std::cout << remote.url() << std::endl;
    }

     git::repository inited(git::init("/home/nick/Projects/fyrware/git/ignore/inited"));

    git::util::uninitialize();
}