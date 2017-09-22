cmake_minimum_required(VERSION 3.8)

project(Lexer)

find_package(FLEX)
FLEX_TARGET(Flexer flexer.l ${CMAKE_CURRENT_BINARY_DIR}/../flexer.cpp)
message(${FLEX_VERSION})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -Wall -Wno-conversion -Wno-deprecated-register")
# add brewed flex include
set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -I/usr/local/opt/flex/include")

set(SOURCE_FILES hlexer.cpp ${FLEX_Flexer_OUTPUTS} main.cpp hlexer.h flexer.h lexer.h symbol_table.h)
add_executable(lexer ${SOURCE_FILES})
