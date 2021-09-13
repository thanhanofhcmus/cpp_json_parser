cmake_minimum_required(VERSION 3.12.0)

add_library(${PROJECT_NAME})

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/JsonValue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
    ${PJ_INCLUDE_DIR}/json_parser/core.h
    ${PJ_INCLUDE_DIR}/json_parser/JsonException.h
    ${PJ_INCLUDE_DIR}/json_parser/JsonValue.h
    ${PJ_INCLUDE_DIR}/json_parser/detail/Token.h
    ${PJ_INCLUDE_DIR}/json_parser/detail/Scanner.h
    ${PJ_INCLUDE_DIR}/json_parser/detail/Parser.h
)
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE project_warnings)
target_link_libraries(${PROJECT_NAME} PUBLIC project_options)
