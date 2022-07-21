cmake_minimum_required(VERSION 3.22)
project(commons-unit-test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

file(GLOB tests_SRC tests/utils/*.c tests/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/../../src lib)
link_directories(${CMAKE_SOURCE_DIR}/../../src/build)

add_executable(${PROJECT_NAME} tests/main.cpp ${tests_SRC})
target_link_libraries(${PROJECT_NAME} commons)
