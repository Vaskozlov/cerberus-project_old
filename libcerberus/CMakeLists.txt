include(.cmake-rules)
include_directories(libcerberus/include)

file(
        GLOB_RECURSE
        CATCH_OBJS
        "libcerberus/catch/*.cpp"
)

add_executable(
        catch
        ${CATCH_OBJS}
)

target_link_libraries(
        catch
        fmt::fmt
)

add_test(LIBCERBERUS_TEST catch)