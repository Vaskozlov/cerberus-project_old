include(.cmake-rules)
include_directories(libcerberus/include)

file(
        GLOB_RECURSE
        LIBRARY_CATCH_OBJS
        "libcerberus/catch/*.cpp"
)

add_executable(
        library_catch
        ${LIBRARY_CATCH_OBJS}
)

target_link_libraries(
        library_catch
        fmt::fmt
)

add_test(LIBCERBERUS_TEST library_catch)