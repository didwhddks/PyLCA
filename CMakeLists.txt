cmake_minimum_required(VERSION 3.14)
project(PyLCA)

set(CMAKE_CXX_STANDARD 17)
find_package(pybind11 REQUIRED)

# your core library
add_library(lca_core
  src/factory.cpp
  src/BinaryLiftingLCA.cpp
  src/RMQLCA.cpp
  src/Optimized_RMQLCA.cpp
  src/TarjanLCA.cpp
)
target_include_directories(lca_core PUBLIC include)

# the Python extension
pybind11_add_module(pylca python/lca_bindings.cpp)
target_link_libraries(pylca PRIVATE lca_core)