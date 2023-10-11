cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
set(MY_PROJECT_NAME ip_filter)
set(EXEC_FILE_NAME ip_filter_cli)

project(${MY_PROJECT_NAME} VERSION ${PROJECT_VESRION})


add_executable(${EXEC_FILE_NAME} ip_filter.cpp)

set_target_properties(${EXEC_FILE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(${EXEC_FILE_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${EXEC_FILE_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ${EXEC_FILE_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
