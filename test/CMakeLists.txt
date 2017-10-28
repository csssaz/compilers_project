set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-I/usr/local/opt/flex/include")
set(CMAKE_CXX_STANDARD 11)

include_directories(${Compilers_SOURCE_DIR}/)
include_directories(${Compilers_SOURCE_DIR}/lexer)

set(TEST_SRC_PARSER ${Compilers_SOURCE_DIR}/parser_decaf.cpp ${Compilers_SOURCE_DIR}/lexer_decaf.cpp  ${Compilers_SOURCE_DIR}/hparser.h ${Compilers_SOURCE_DIR}/hparser.cpp )
set(TEST_FILES_PARSER test_parser.cpp)
add_executable(test_parser ${TEST_FILES_PARSER} ${TEST_SRC_PARSER})

target_link_libraries(test_parser Catch)

set(TEST_SRC_LEXER  ${Compilers_SOURCE_DIR}/lexer/hlexer.cpp ${Compilers_SOURCE_DIR}/lexer/regex.cpp ${Compilers_SOURCE_DIR}/lexer/flexer.h ${Compilers_SOURCE_DIR}/lexer/flexer.cpp)
set(TEST_FILES_LEXER testmain.cpp)
add_executable(test_lexer ${TEST_FILES_LEXER} ${TEST_SRC_LEXER})
target_link_libraries(test_lexer Catch)




