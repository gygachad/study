cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Google test" ON)

file(READ not_filtered.tsv NOT_FILTERED)
file(READ filtered.tsv FILTERED)

configure_file(gtest_ipfilter.h.in gtest_ipfilter.h)
configure_file(version.h.in version.h)

file(COPY ${CMAKE_BINARY_DIR}/version.h
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

file(COPY ${CMAKE_BINARY_DIR}/gtest_ipfilter.h
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ip_filter main.cpp)
add_library(ip_filter_lib ip_addr.cpp ip_pool.cpp version.cpp str_tool.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(ip_filter PRIVATE
    ip_filter_lib
)

if(WITH_GOOGLE_TEST)

    find_package(GTest REQUIRED)
    add_executable(gtest_ipfilter gtest_ipfilter.cpp)

    set_target_properties(gtest_ipfilter PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    include_directories(${GTEST_INCLUDE_DIR})

    target_link_libraries(gtest_ipfilter
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
        ip_filter_lib
    )

    if(UNIX)
        find_package(Threads REQUIRED)
        target_link_libraries(gtest_ipfilter
            ${CMAKE_THREAD_LIBS_INIT}
        )
    endif()

endif()

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(ip_filter_lib PRIVATE
            /MT
        )
        target_compile_options(ip_filter PRIVATE
            /MT
        )
        target_compile_options(gtest_ipfilter PRIVATE
            /MT
        )
    endif()
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

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
    add_test(gtest_ipfilter gtest_ipfilter)
endif()