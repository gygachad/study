cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(matrix VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Google test" ON)

configure_file(version.h.in version.h)

file(COPY ${CMAKE_BINARY_DIR}/version.h
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(matrix main.cpp version.cpp)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(matrix
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(WITH_GOOGLE_TEST)

    find_package(GTest REQUIRED)
    add_executable(gtest_matrix gtest_matrix.cpp version.cpp)

    set_target_properties(gtest_matrix PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    include_directories(
        ${GTEST_INCLUDE_DIR}
    )

    target_link_libraries(gtest_matrix
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
    )

    if(UNIX)
        find_package(Threads REQUIRED)
        target_link_libraries(gtest_matrix
            ${CMAKE_THREAD_LIBS_INIT}
        )        
    endif()

endif()

if (MSVC)
    target_compile_options(matrix PRIVATE
        /W4
    )
    if(WITH_GOOGLE_TEST)
        target_compile_options(matrix PRIVATE
            /MT
        )
        target_compile_options(gtest_matrix PRIVATE
            /MT
        )
    endif()
else ()
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS matrix RUNTIME DESTINATION bin)

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
    add_test(gtest_matrix gtest_matrix)
endif()