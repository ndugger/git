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
    git::manager::init();

    git::repository repository(git::open("/home/nick/Projects/fyrware/git/delete_this/repository"));
    git::remote origin(repository.remote("origin"));

    origin.fetch();

    git::branch master(repository.branch("master"));
    git::branch origin_master(origin.branch("master"));

    repository.set_head(master);
    repository.merge(master, origin_master);

    git::manager::clean_up();
}