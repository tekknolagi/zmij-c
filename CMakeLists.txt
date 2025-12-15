cmake_minimum_required(VERSION 3.16)
project(zmij C)

add_library(zmij zmij.c zmij.h)
target_compile_features(zmij PRIVATE c_std_11)

add_executable(example example.c)
target_link_libraries(example zmij)

set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
enable_testing()

add_executable(zmij-test zmij-test.c)
target_link_libraries(zmij-test zmij m)
add_test(NAME zmij-test COMMAND zmij-test)
