cmake_minimum_required(VERSION 3.15.0)

project(cerberus VERSION 0.0.1.0 LANGUAGES CXX)
enable_testing()

option(CERBERUS_STRICT_COMPILATION "Enable -Werror strict compilation" OFF)

message(STATUS "CC " ${CMAKE_C_COMPILER})
message(STATUS "CXX " ${CMAKE_CXX_COMPILER})
message(STATUS "Strict compilation? " ${CERBERUS_STRICT_COMPILATION})

include(libcerberus/CMakeLists.txt)
