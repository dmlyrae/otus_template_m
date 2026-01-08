cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(print_ip VERSION ${PROJECT_VERSION})

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

# --- Определение целей (библиотека и исполняемый файл) ---
add_library(print_ip_lib src/lib.cpp)
add_executable(print_ip src/main.cpp)

# Устанавливаем стандарт C++
set_target_properties(print_ip print_ip_lib PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(print_ip PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}"
)
target_include_directories(print_ip_lib PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_BINARY_DIR}"
)

target_link_libraries(print_ip PRIVATE print_ip_lib)

install(TARGETS print_ip RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT "dm.lyrae@ya.ru")
include(CPack)

enable_testing()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tests")
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/tests")
endif()

add_test(
    NAME output_check_test
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test_1.sh $<TARGET_FILE:print_ip>
)