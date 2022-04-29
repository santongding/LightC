cmake_minimum_required(VERSION 3.21)
project(LightC)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME}_lib STATIC
        ${PROJECT_SOURCE_DIR}/bison/lex.yy.cc
        tac/tac_exp.cpp
        )

add_executable(LightC ${PROJECT_SOURCE_DIR}/bison/bison.cpp )

target_link_libraries(LightC ${PROJECT_NAME}_lib)