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
# include "git/util.cpp"

int main () {
    git::manager::init();

    git::repository repository_cloned(git::clone("https://github.com/fyrware/git.git", "/home/nick/Projects/fyrware/git/ignore/git/"));
    git::branch branch_local_master(repository_cloned.branch("master"));

    repository_cloned.set_head(branch_local_master);

    git::remote remote_origin(repository_cloned.remote("origin"));

    remote_origin.fetch(); // TODO this is where it currently fails

    git::branch branch_origin_master(remote_origin.branch("master"));

    repository_cloned.merge(branch_origin_master);

    git::manager::clean_up();
}