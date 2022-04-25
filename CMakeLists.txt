cmake_minimum_required(VERSION 3.15.0)

project(cerberus VERSION 0.0.1.0 LANGUAGES CXX)
enable_testing()

option(CERBERUS_STRICT_COMPILATION "Enable -Werror strict compilation" OFF)
option(USE_LIBCPP "Use libc++ when compiling with clang" OFF)
option(USE_SANITIZERS "Use sanitizers" ON)
option(USE_DYNAMIC_LIBRARY "Compile dynamic linking version of cerberus library" ON)

message(STATUS "CC " ${CMAKE_C_COMPILER})
message(STATUS "CXX " ${CMAKE_CXX_COMPILER})
message(STATUS "Strict compilation? " ${CERBERUS_STRICT_COMPILATION})
message(STATUS "Use libcpp? " ${USE_LIBCPP})
message(STATUS "Use sanitizers? " ${USE_SANITIZERS})
message(STATUS "Dynamic linking version? " ${USE_DYNAMIC_LIBRARY})

include(libcerberus/CMakeLists.txt)
include(analysis/CMakeLists.txt)
