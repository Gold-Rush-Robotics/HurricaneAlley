cmake_minimum_required(VERSION 3.0.0)
project(CAR VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


include_directories(./Utilities/ ../../rpidmx512/lib-pca9685/include/ ../OldCode/ Opmodes/ Robot/ HardwareInterfaces/ Robot/mechanisms/)

include_directories(../eigen/)

set(HARDWARE HardwareInterfaces/motor.cpp HardwareInterfaces/servo.cpp HardwareInterfaces/octoquad.cpp HardwareInterfaces/encoder.cpp)
set(ROBOT Robot/drivetrain.cpp Robot/robot.cpp Robot/mechanisms/revolver.cpp)
set(UTILITIES Utilities/utilFunc.cpp Utilities/I2C.cpp)
set(TELEOP Opmodes/Teleop.cpp Opmodes/Runner.cpp)
set(AUTO Opmodes/Auto.cpp Opmodes/Runner.cpp)
set(SIMPLE Opmodes/SimpleDrive.cpp)

set(SOURCES ${HARDWARE} ${ROBOT} ${UTILITIES})

add_executable(GoldRushTele ${SOURCES} ${TELEOP})
add_executable(GoldRushAuto ${SOURCES} ${AUTO})
add_executable(GoldRushDrive ${SOURCES} ${SIMPLE})

target_link_libraries(GoldRushTele -L"../../rpidmx512/lib-pca9685/lib_linux/" -lpigpio -lrt -lpca9685 -lbcm2835 -lpthread)
target_link_libraries(GoldRushAuto -L"../../rpidmx512/lib-pca9685/lib_linux/" -lpigpio -lrt -lpca9685 -lbcm2835 -lpthread)
target_link_libraries(GoldRushDrive -L"../../rpidmx512/lib-pca9685/lib_linux/" -lpigpio -lrt -lpca9685 -lbcm2835 -lpthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
