# git
C++ Wrapper Around [libgit2](https://github.com/libgit2/libgit2)

### where?
```
grok use git
```

### why?
libgit2 is a great library. The intellectual barrier for entry is high, though. 
There is a lot of cognitive "hot potato" that one needs to play when writing C. 
It's a good thing, mostly, but we want to ramp developers up for that; the deep end is scary. 
For that reason, I've decided to write an OOP-ish wrapper around libgit2 with C++ and its STL.

### how?
![](https://i.imgur.com/BZfr4YM.png)

```cpp
int main () {
    git::util::initialize();

    git::repository repository_cloned(git::clone("https://address/to/repository.git", "/path/to/directory/"));
    git::branch branch_local_master(repository_cloned.branch("master"));

    repository_cloned.set_head(branch_local_master);

    git::remote remote_origin(repository_cloned.remote("origin"));

    remote_origin.fetch();

    git::branch branch_origin_master(remote_origin.branch("master"));

    repository_cloned.merge(branch_origin_master);

    git::util::uninitialize();
}
```
