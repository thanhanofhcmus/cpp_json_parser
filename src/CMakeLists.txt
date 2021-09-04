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

# Copy from cpp_stater_project of lefticus.
# See: https://github.com/cpp-best-practices/cpp_starter_project/blob/main/cmake/CompilerWarnings.cmake
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
    /permissive- # standards conformance mode for MSVC compiler.
    /W4     # Baseline reasonable warnings
    /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
    /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
    /w14263 # 'function': member function does not override any base class virtual member function
    /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
            # be destructed correctly
    /w14287 # 'operator': unsigned/negative constant mismatch
    /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside
            # the for-loop scope
    /w14296 # 'operator': expression is always 'boolean_value'
    /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
    /w14545 # expression before comma evaluates to a function which is missing an argument list
    /w14546 # function call before comma missing argument list
    /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
    /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
    /w14555 # expression has no effect; expected expression with side- effect
    /w14619 # pragma warning: there is no warning number 'number'
    /w14640 # Enable warning on thread un-safe static member initialization
    /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
    /w14905 # wide string literal cast to 'LPSTR'
    /w14906 # string literal cast to 'LPWSTR'
    /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wshadow -Wnon-virtual-dtor 
    -Wold-style-cast -Wcast-align -Wunused -Wpedantic -Wconversion 
    -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat 
    -Wmissing-braces -Wmisleading-indentation -Wduplicated-cond 
    -Wduplicated-branches -Wlogical-op -Wredundant-move -Wpessimizing-move
    )
 else()
    message("No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif()
