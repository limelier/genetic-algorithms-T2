cmake_minimum_required(VERSION 3.15)
project(cpp_proj)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_proj main.cpp util/bitset.cpp util/bitset.h util/functions.cpp util/functions.h util/result.h util/random.cpp util/random.h src/testRunner.cpp src/testRunner.h config.h src/methods.cpp src/methods.h)