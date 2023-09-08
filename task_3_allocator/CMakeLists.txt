cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(allocator VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Google test" ON)
option(WITH_GOOGLE_BENCHMARK "Google benchmark" ON)

configure_file(version.h.in version.h)

file(COPY ${CMAKE_BINARY_DIR}/version.h
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(allocator main.cpp version.cpp)

set_target_properties(allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(allocator
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(WITH_GOOGLE_TEST)

    find_package(GTest REQUIRED)
    add_executable(gtest_allocator gtest_allocator.cpp version.cpp)

    set_target_properties(gtest_allocator PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    include_directories(
        ${GTEST_INCLUDE_DIR}
    )

    target_link_libraries(gtest_allocator
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
    )

    if(UNIX)
        find_package(Threads REQUIRED)
        target_link_libraries(gtest_allocator
            ${CMAKE_THREAD_LIBS_INIT}
        )        
    endif()

endif()

if(WITH_GOOGLE_BENCHMARK)
    find_package(benchmark REQUIRED)
    add_executable(gbenchmark_allocator gbenchmark_allocator.cpp version.cpp)

    set_target_properties(gbenchmark_allocator PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    if(UNIX)
        find_package(Threads REQUIRED)
        target_link_libraries(gbenchmark_allocator
            -lbenchmark
        )
    else()
        include_directories(
            ${GBENCHMARK_ROOT}/include
        )
    
        target_link_libraries(gbenchmark_allocator
            ${GBENCHMARK_ROOT}lib/benchmark.lib
            ${GBENCHMARK_ROOT}lib/benchmark_main.lib
        )
    endif()

endif()

if (MSVC)
    target_compile_options(allocator PRIVATE
        /W4
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(allocator PRIVATE
            /MT
        )
        target_compile_options(gtest_allocator PRIVATE
            /MT
        )
    endif()
    if(WITH_GOOGLE_BENCHMARK)
        target_compile_options(gbenchmark_allocator PRIVATE
            /MD
        )
    endif()
else ()
    target_compile_options(allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

if (WIN32)
    set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_GOOGLE_TEST)
    enable_testing()
    add_test(gtest_allocator gtest_allocator)
    add_test(gbenchmark_allocator gbenchmark_allocator)
endif()