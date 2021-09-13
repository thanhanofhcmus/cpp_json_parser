cmake_minimum_required(VERSION 3.15.0)

if (TEST_USE_FetchContent)
    Include(FetchContent)
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.0.0-preview3
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
else()
    find_package(Catch2 REQUIRED)
endif()

include(CTest)
include(Catch)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(tests json_value.cpp scanner.cpp)
target_link_libraries(tests PRIVATE
  project_warnings
  catch_main
  json_parser
)

catch_discover_tests(tests)
