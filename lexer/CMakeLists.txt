cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_FLAGS "-I/usr/local/opt/flex/include -Wall -Wextra -ansi -pedantic")
find_package(FLEX)
message(${CMAKE_CURRENT_SOURCE_DIR})
set(FLEX_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${FLEX_INCLUDE_DIRS}) 
FLEX_TARGET(Flexer flexer.l ${CMAKE_CURRENT_SOURCE_DIR}/flexer.cpp)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -Wall -Wno-conversion -Wno-deprecated-register")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/hregex.in ${CMAKE_CURRENT_BINARY_DIR}/hregex.in COPYONLY)

set(SOURCE_FILES main.cpp hlexer.cpp ${FLEX_Flexer_OUTPUTS} flexer.h lexer.h symbol_table.h token.h regex.cpp)
add_executable(Compilers ${SOURCE_FILES})

