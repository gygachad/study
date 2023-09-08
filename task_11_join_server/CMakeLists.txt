cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

file(COPY ${CMAKE_BINARY_DIR}/version.h
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
		
add_executable(join_server main.cpp asio_server.cpp join_server.cpp str_tool.cpp version.cpp)

set_target_properties(join_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(join_server
    PRIVATE "${CMAKE_BINARY_DIR}" ${ASIO_INCLUDE_DIR}
)

if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_definitions(join_server PRIVATE _WIN32_WINNT=0x0601)
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_link_libraries(join_server pthread)
endif()

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

if (WIN32)
    set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)