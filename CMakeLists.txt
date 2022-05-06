cmake_minimum_required(VERSION 3.21)
project(LightC)

set(shared_include_dir  ${PROJECT_SOURCE_DIR}/shared_include)
set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(arm64_include_dir ${PROJECT_SOURCE_DIR}/asm_include/arm64)
include_directories(${include_dir})
include_directories(${arm64_include_dir})
include_directories(${shared_include_dir})

add_library(${PROJECT_NAME}_lib STATIC
        src/bison/lex.yy.cc
        src/tac/tac_exp.cpp
        src/tac/tac_base.cpp
        src/tac/tac_block.cpp
        src/tac/tac_func.cpp
        src/checker/Checker.cpp
        src/checker/TypeManager.cpp
        src/utils.cpp
        src/object/AsmDump.cpp
        src/object/object.cpp
        src/checker/TypeInfo.cpp
        src/linker/linker.cpp
        )

add_executable(LightC src/bison/bison.cpp)

target_link_libraries(LightC ${PROJECT_NAME}_lib)
add_subdirectory(test)
enable_testing()
add_test(test ${CMAKE_SOURCE_DIR}/build/test/LightC_test)