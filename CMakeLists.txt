cmake_minimum_required(VERSION 3.8)
project(Compilers)

add_subdirectory(test)

set(CMAKE_CXX_FLAGS "-I/usr/local/opt/flex/include")
find_package(FLEX)
FLEX_TARGET(Flexer flexer.l ${CMAKE_CURRENT_BINARY_DIR}/../flexer.cpp)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -Wall -Wno-conversion -Wno-deprecated-register")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/hregex.in ${CMAKE_CURRENT_BINARY_DIR}/hregex.in COPYONLY)

set(SOURCE_FILES main.cpp hlexer.cpp ${FLEX_Flexer_OUTPUTS} hlexer.h flexer.h lexer.h symbol_table.h token.h regex.cpp)
add_executable(Compilers ${SOURCE_FILES})

