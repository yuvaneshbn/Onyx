#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hidapi::winapi" for configuration "RelWithDebInfo"
set_property(TARGET hidapi::winapi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(hidapi::winapi PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/hidapi.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/hidapi.dll"
  )

list(APPEND _cmake_import_check_targets hidapi::winapi )
list(APPEND _cmake_import_check_files_for_hidapi::winapi "${_IMPORT_PREFIX}/lib/hidapi.lib" "${_IMPORT_PREFIX}/bin/hidapi.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
