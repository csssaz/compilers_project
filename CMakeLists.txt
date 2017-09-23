cmake_minimum_required(VERSION 3.8)
project(Project1)

find_package(FLEX)
FLEX_TARGET(Flexer flexer.l ${CMAKE_CURRENT_BINARY_DIR}/../flexer.cpp)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -Wall -Wno-conversion -Wno-deprecated-register")

set(SOURCE_FILES main.cpp hlexer.cpp ${FLEX_Flexer_OUTPUTS} hlexer.h flexer.h lexer.h symbol_table.h token.h)
add_executable(Project1 ${SOURCE_FILES})