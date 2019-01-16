# include <iostream>
# include <string>

# include "example/fetch_merge.cpp"

# include "git/clone.cpp"
# include "git/commit.cpp"
# include "git/init.cpp"
# include "git/manager.cpp"
# include "git/open.cpp"
# include "git/remote.cpp"
# include "git/repository.cpp"
# include "git/tree.cpp"

int main () {
    fetch_merge::main(); // c++ layer might actually be working -- try repo via https (no ssh -- need to recompile libgit2)
}