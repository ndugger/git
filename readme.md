# git**
C++ Wrapper Around [libgit2](https://github.com/libgit2/libgit2)

**\*\*Currently broken fetch/merge operation**

### where?
[grok](https://github.com/fyrware/grok) package manager
```
grok use git
```

### why?
libgit2 is a great library. The intellectual barrier for entry is high, though. 
There is a lot of cognitive "hot potato" that one needs to play when writing C. 
It's a good thing, mostly, but we want to ramp developers up for that; the deep end is scary. 
For that reason, I've decided to write an OOP-ish wrapper around libgit2 with C++ and its STL.

### how?
[example/main.cpp](https://github.com/fyrware/git/blob/master/example/main.cpp)
![screenshot of code](https://i.imgur.com/YR59GEz.png)
