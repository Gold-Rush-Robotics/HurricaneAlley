cmake_minimum_required(VERSION 3.0.0)
project(CAR VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


include_directories(./Utilities/ ../../rpidmx512/lib-pca9685/include/ ../OldCode/ Robot/HardwareInterfaces/)

set(HARDWARE HardwareInterfaces/motor.cpp HardwareInterfaces/octoquad.cpp HardwareInterfaces/encoder.cpp)
set(ROBOT Robot/drivetrain.cpp Robot/robot.cpp)
set(UTILITIES Utilities/utilFunc.cpp Utilities/I2C.cpp)
set(RUNNER Teleop.cpp)

set(SOURCES ${HARDWARE} ${ROBOT} ${UTILITIES} ${RUNNER})

add_executable(CART ${SOURCES})
target_link_libraries(CART -L"../../rpidmx512/lib-pca9685/lib_linux/" -lpigpio -lrt -lpca9685 -lbcm2835 -lpthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
