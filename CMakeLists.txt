cmake_minimum_required(VERSION 3.16)
project(LightC)
include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(/usr/include)

add_library(${PROJECT_NAME}_lib STATIC
        src/bison/lex.yy.cc
        src/tac/tac_exp.cpp
        src/tac/tac_base.cpp
        src/tac/tac_block.cpp
        src/tac/tac_func.cpp
        src/checker/Checker.cpp
        src/checker/TypeManager.cpp
        src/utils.cpp
        )

add_executable(LightC src/bison/bison.cpp)

target_link_libraries(LightC ${PROJECT_NAME}_lib)
