cmake_minimum_required(VERSION 3.12.0)

add_executable(${PROJECT_NAME})

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JsonValue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
    ${PROJECT_SOURCE_DIR}/include/core.h
    ${PROJECT_SOURCE_DIR}/include/JsonException.h
    ${PROJECT_SOURCE_DIR}/include/JsonValue.h
    ${PROJECT_SOURCE_DIR}/include/Token.h
    ${PROJECT_SOURCE_DIR}/include/Scanner.h
    ${PROJECT_SOURCE_DIR}/include/Parser.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

if (MSVC)

else()
    target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wshadow -Wnon-virtual-dtor 
    -Wold-style-cast -Wcast-align -Wunused -Wpedantic -Wconversion 
    -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat 
    -Wmissing-braces -Wmisleading-indentation -Wduplicated-cond 
    -Wduplicated-branches -Wlogical-op -Wredundant-move -Wpessimizing-move
    )
endif()