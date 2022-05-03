cmake_minimum_required(VERSION 3.21)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/asm_include/arm64)
set(PROJECT_TEST_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/test/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_TEST_BINARY_DIR})

add_executable(${PROJECT_NAME}_test ./test.cpp)

target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_lib)
