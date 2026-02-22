# This is a new branch that will be a rewrite of Tapr

### This rewrite aims to remove unecessary redundancies and hard-coded spaghetti code from the main branch, and make it cross platform too lol

## dependencies
- C++ compiler (GCC for unix, MinGW for windows)
- CMake ([cmake](https://cmake.org/cmake/download))

Building
===
## Unix
```cmd
$ mkdir build
$ cd build
$ cmake ..
$ make
```
and to run
```
$ ./Tapr
```

## Windows
```cmd
$ mkdir build
$ cd build
$ cmake .. -G "MinGW Makefiles"
$ make
```
and to run
```
$ Tapr.exe
```

