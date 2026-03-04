# Tapr 2.0

> [!WARNING]
> incomplete!

## dependencies
- CMake ([cmake](https://cmake.org/cmake/download))
- C++ compiler (GCC for unix, MinGW for windows)

Building
===
## Unix
```cmd
$ mkdir build
$ cd build
$ cmake .. -DRELEASE=ON
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
$ cmake .. -G "MinGW Makefiles" -DRELEASE=ON
$ make
```
and to run
```
$ Tapr.exe
```

TODO
===
* SCORE SAVING
* MORE JUICE


