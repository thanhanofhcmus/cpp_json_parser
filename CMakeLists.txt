cmake_minimum_required(VERSION 3.15.0)
include(cmake/StandardSettings.cmake)
project(json_parser VERSION 0.1.0)

set(PJ_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# this library is for reusing compiler options
add_library(project_options INTERFACE)
target_include_directories(project_options INTERFACE ${PJ_INCLUDE_DIR})

# this library is for reusing warnings
add_library(project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_subdirectory(src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    option(TEST_USE_FectchContent "Use FectContent instead of pre-installed catch2" OFF)
    add_subdirectory(test)
endif()
