cmake_minimum_required(VERSION 3.21)
project(LightC)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/usr/include)

add_library(${PROJECT_NAME}_lib STATIC
        ${PROJECT_SOURCE_DIR}/bison/lex.yy.cc
        tac/tac_exp.cpp
        tac/tac_base.cpp
        tac/tac_block.cpp
        tac/tac_func.cpp
        checker/Checker.cpp
        checker/TypeManager.cpp
        )

add_executable(LightC ${PROJECT_SOURCE_DIR}/bison/bison.cpp )

target_link_libraries(LightC ${PROJECT_NAME}_lib)