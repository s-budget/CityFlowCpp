#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CityFlow::cityflow_lib" for configuration "RelWithDebInfo"
set_property(TARGET CityFlow::cityflow_lib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(CityFlow::cityflow_lib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/cityflow_lib.lib"
  )

list(APPEND _cmake_import_check_targets CityFlow::cityflow_lib )
list(APPEND _cmake_import_check_files_for_CityFlow::cityflow_lib "${_IMPORT_PREFIX}/lib/cityflow_lib.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
