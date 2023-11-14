##########################################################################
cmake_minimum_required(VERSION 2.8)
##########################################################################
project(can-test)
##########################################################################
include_directories(../../src)
include_directories(../../../../cores/arduino)
include_directories(external/catch2/v2.13.10/include)
##########################################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
##########################################################################
set(TEST_TARGET ${CMAKE_PROJECT_NAME})
##########################################################################
set(TEST_TARGET_SRCS
  src/test_calc_can_bit_timinig.cpp
  src/test_main.cpp
  ../../src/CanUtil.cpp
)
##########################################################################
add_compile_options(-Wall -Wextra -Wpedantic)
##########################################################################
set(CMAKE_C_FLAGS   ${CMAKE_C_FLAGS}   "--coverage")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--coverage -Wno-deprecated-copy")
##########################################################################
add_executable(
  ${TEST_TARGET}
  ${TEST_TARGET_SRCS}
)
##########################################################################
