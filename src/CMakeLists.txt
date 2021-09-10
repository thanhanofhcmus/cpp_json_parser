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
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE project_options project_warnings)
