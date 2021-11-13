cmake_minimum_required(VERSION 3.15.0)

project(libcerberus VERSION 0.0.1.0 LANGUAGES CXX)

include(../.cmake-rules)
include_directories(include)

file(
        GLOB_RECURSE
        CATCH_OBJS
        "catch/*.cpp"
)

add_executable(
        catch
        ${CATCH_OBJS}
)

target_link_libraries(
        catch
        fmt::fmt
)
