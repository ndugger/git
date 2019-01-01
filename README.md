# git
C++ Wrapper Around libgit2

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
```cpp
# include "git/clone.cpp"
# include "git/repository.cpp"

namespace example {

    void main () {
        git::repository repository(git::clone("https://url/to/repository.git"));
        
        for (git::remote& remote : repository.remotes()) {
            // ...
        }
    }
}

int main () {
    example::main();
}
```