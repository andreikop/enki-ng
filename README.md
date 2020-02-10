# Enki Editor (next generation)

Work in progress.
[Previous generation](https://github.com/andreikop/enki) is more usable now.

## Build dependencies
1. Install `g++`
2. Install `cmake`
2. Install Qt5 development files.
2. Install [Qutepart C++ version](https://github.com/andreikop/qutepart-cpp)

On Ubuntu and Ubuntu-based systems

```
    sudo apt-get install g++ cmake qt5base-dev qtbase5-dev-tools
```

## Building with Qutepart installed to the system

### Prepare build directory

```
    mkdir build
    cd build
```

### Configure

To use Qutepart installed to the system:

```
    cmake ..
```

It is also possible to build Enki with Qutepart checked out as source tree.

This variant usually is used for development

```
    cmake .. -DCMAKE_SRC_DIR=../../qutepart-cpp/
```

Replace `../../qutepart-cpp` with your Qutepart source directory path relative to current directory.

### Build

```
    make
```

### To run from the sources
```
    ./enki-ng
```
(From the build directory, after enki-ng was built)

### To install to the system
```
    sudo make install
```
