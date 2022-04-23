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

if (USE_DYNAMIC_LIBRARY)
    add_library(
            analysis_lib
            SHARED
            ${ANALYSIS_OBJ}
    )

    set_target_properties(analysis_lib PROPERTIES COMPILE_FLAGS "-fpic")
else ()
    add_library(
            analysis_lib
            STATIC
            ${ANALYSIS_OBJ}
    )
endif ()

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
