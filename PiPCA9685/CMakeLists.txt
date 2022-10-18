cmake_minimum_required(VERSION 3.1)
project(PiPCA9685 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

#
# Dependencies
#
include_directories(./include/PiPCA9685)
#
# LIBRARY
#

add_library(PiPCA9685 src/PCA9685.cpp src/I2CPeripheral.cpp)
target_link_libraries(PiPCA9685 i2c)
set_property(TARGET PiPCA9685 PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(PiPCA9685 PROPERTIES VERSION ${PiPCA9685_VERSION} SOVERSION ${PiPCA9685_VERSION})



#
# C++ INSTALLATION
#

set(SOURCES include/PCA9685.h)
set(INSTALL_INCLUDE_DIR "include/")
set(INSTALL_LIBS_DIR "lib")
set(INSTALL_STATIC_DIR "lib/static")
set(INSTALL_RUNTIME_DIR "bin")
set(INSTALL_CONFIG_DIR "${INSTALL_LIBS_DIR}/PiPCA9685/cmake")
set(HEADER_FILES include/PCA9685.h)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/PiPCA9685Config.cmake.in
        PiPCA9685Config.cmake
    INSTALL_DESTINATION lib/cmake/PiPCA9685
    PATH_VARS INSTALL_INCLUDE_DIR
)

install(
    TARGETS PiPCA9685
    EXPORT PiPCA9685Targets
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBS_DIR}
    ARCHIVE DESTINATION ${INSTALL_STATIC_DIR}
)

install(
    DIRECTORY include/PiPCA9685
    DESTINATION ${INSTALL_INCLUDE_DIR}
)

install(
    EXPORT PiPCA9685Targets
    FILE PiPCA9685Targets.cmake
    DESTINATION ${INSTALL_CONFIG_DIR}
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/PiPCA9685Config.cmake"
    DESTINATION ${INSTALL_CONFIG_DIR}
)

