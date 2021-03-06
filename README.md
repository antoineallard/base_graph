# BaseGraph: A portable graph library

BaseGraph provides a set of C++ objects and scripts to analyze, manipulate and simulate the structure of graphs (or [complex networks]).

Most of the core functionalities of the C++ library are available as a Python module. Objects and functions are wrapped using [pybind11].

## Why another graph library?

The development of this library was motivated by the need of a __fast__, __lightweight__ and __customizable__ code, which was __easy to install__ and straightforward to deploy on __supercomputing facilities__ whose softwares are not always up-to-date.

At the beginning of this project, none of the existing high-quality libraries such as [graph-tool], [NetworkX] or [igraph] fulfilled the aforementionned requirements.

I therefore decided to start writing my own code, adding new features following the needs of my research. This is a work in progress; comments, suggestions, contributions and bug reports are welcomed.

## Requirements

  * A C++11 (or newer) compliant compiler
  * [CMake] 3.17+ for the C++ library
  * Python 3.X for the Python module
  * [pybind11] for the Python module


## Installation
Clone this repository
```
git clone https://github.com/antoineallard/base_graph.git
```

#### C++
To compile the static library, execute
```sh
mkdir build && cd build && cmake .. && make && cd ..
```
The static library will appear as `build/src/libBaseGraph.a` (the extension will vary depending on the operating system).

To link the library in another CMake project, use
```cmake
include_directories(/path/to/the/repository/include)
...
target_link_libraries(<TARGET> /path/to/the/repository/build/src/libBaseGraph.a)
```
A small example is provided in the ``example`` directory.

#### Python wrapper
After cloning the repository, execute
```sh
pip install .
```

## Testing
In order to build the unit tests, the option `BUILD_TESTS` must be toggled on
```
cmake -DBUILD_TESTS=on ..
```
The option is saved in the CMake cache so it only needs to be set once.

The [GoogleTest] framework is used to manage the unit tests. GoogleTest does not need to be installed because it is automatically pulled from GitHub by the CMake build.

[Boost]:            https://www.boost.org
[CMake]:            https://cmake.org
[complex networks]: https://en.wikipedia.org/wiki/Complex_network
[Eigen]:            http://eigen.tuxfamily.org
[graph-tool]:       https://graph-tool.skewed.de/
[igraph]:           https://igraph.org/
[NetworkX]:         https://networkx.github.io/
[pybind11]:         https://github.com/pybind/pybind11
[Spectra]:          https://spectralib.org/
[STL]:              https://en.cppreference.com/w
[GoogleTest]:      https://github.com/google/googletest
