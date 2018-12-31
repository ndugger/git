# include "git/clone.cpp"
# include "git/repository.cpp"
# include "git/util.cpp"

int main () {
    git::util::initialize();
    git::clone("https://github.com/fyrware/git.git", "/home/nick/Projects/fyrware/git/.trash/");
    git::util::uninitialize();
}