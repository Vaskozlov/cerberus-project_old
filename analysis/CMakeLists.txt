include_directories(analysis/include)

file(
        GLOB_RECURSE
        ANALYSIS_OBJ
        "analysis/src/*.cpp"
)

file(
        GLOB_RECURSE
        ANALYSIS_CATCH_OBJ
        "analysis/catch/*.cpp"
)

add_executable(
        lexical_analysis_catch
        ${ANALYSIS_CATCH_OBJ}
)

target_link_libraries(
        lexical_analysis_catch
        fmt::fmt
)

add_test(LIBCERBERUS_TEST analysis_catch)
