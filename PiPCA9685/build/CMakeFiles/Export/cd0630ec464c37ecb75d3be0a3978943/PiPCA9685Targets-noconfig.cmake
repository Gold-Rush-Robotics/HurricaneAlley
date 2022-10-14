#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PiPCA9685" for configuration ""
set_property(TARGET PiPCA9685 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PiPCA9685 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/static/libPiPCA9685.a"
  )

list(APPEND _cmake_import_check_targets PiPCA9685 )
list(APPEND _cmake_import_check_files_for_PiPCA9685 "${_IMPORT_PREFIX}/lib/static/libPiPCA9685.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
