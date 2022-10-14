cmake_minimum_required(VERSION 3.1)
project(example)

set(PiPCA9685_DIR ../../cmake/)
set(CMAKE_CXX_STANDARD 14)

include_directories(../../include/PiPCA9685)
find_package(PiPCA9685 REQUIRED)

add_executable(example-cpp main.cpp)
target_link_libraries(example-cpp ${PiPCA9685_LIBRARY})