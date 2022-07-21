cmake_minimum_required(VERSION 3.22)
project(unit-tests)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

file(GLOB tests_SRC src/*.c tests/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/../../src)
link_directories(${CMAKE_SOURCE_DIR}/../../src/build)

add_executable(unit-tests src/main.cpp ${tests_SRC})
target_link_libraries(unit-tests commons)
