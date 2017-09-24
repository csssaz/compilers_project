set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "-I/usr/local/opt/flex/include")
set(CMAKE_CXX_STANDARD 11)

include_directories(${Compilers_SOURCE_DIR})

set(TEST_SRC ${Compilers_SOURCE_DIR}/regex.h ${Compilers_SOURCE_DIR}/flexer.h ${Compilers_SOURCE_DIR}/flexer.cpp)
set(TEST_FILES testmain.cpp)
message(${TEST_SRC})
add_executable(my_test ${TEST_FILES} ${TEST_SRC})
target_link_libraries(my_test Catch)




