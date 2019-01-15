# include <iostream>
# include <string>

# include "git/clone.cpp"
# include "git/commit.cpp"
# include "git/init.cpp"
# include "git/manager.cpp"
# include "git/open.cpp"
# include "git/remote.cpp"
# include "git/repository.cpp"
# include "git/tree.cpp"

int main () {
    git::manager::init();

    git::repository repository(git::open("/home/nick/Projects/fyrware/git/"));
    git::remote remote(repository.remote("origin"));

    remote.fetch();

    git::branch local(repository.branch("master"));
    git::branch upstream(remote.branch("master"));

    repository.set_head(local);
    repository.merge(local, upstream);

    git::manager::clean_up();
}