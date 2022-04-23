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

add_library(
        analysis_lib
        ${ANALYSIS_OBJ}
)

add_executable(
        analysis_catch
        ${ANALYSIS_CATCH_OBJ}
)

target_link_libraries(
        analysis_catch
        fmt::fmt
)

target_link_libraries(analysis_catch Threads::Threads)
target_link_libraries(analysis_catch analysis_lib)

add_test(ANALYSIS_TEST analysis_catch)
