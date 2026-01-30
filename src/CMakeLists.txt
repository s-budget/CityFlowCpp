cmake_minimum_required(VERSION 3.10)
project(cityflow)

set(PROJECT_HEADER_FILES
        utility/config.h
        utility/utility.h
        utility/barrier.h
        utility/optionparser.h
        engine/archive.h
        engine/engine.h
        flow/flow.h
        flow/route.h
        roadnet/roadnet.h
        roadnet/trafficlight.h
        vehicle/router.h
        vehicle/vehicle.h
        vehicle/lanechange.h
)

set(PROJECT_SOURCE_FILES
        utility/utility.cpp
        utility/barrier.cpp
        engine/archive.cpp
        engine/engine.cpp
        flow/flow.cpp
        roadnet/roadnet.cpp
        roadnet/trafficlight.cpp
        vehicle/router.cpp
        vehicle/vehicle.cpp
        vehicle/lanechange.cpp)

set(PROJECT_LIB_NAME ${PROJECT_NAME}_lib CACHE INTERNAL "")

find_package(Threads REQUIRED)

# Add RapidJSON include directory
set(RAPIDJSON_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/../external/rapidjson/include")
if(NOT EXISTS ${RAPIDJSON_INCLUDE_DIR})
    message(FATAL_ERROR "RapidJSON not found at ${RAPIDJSON_INCLUDE_DIR}. Please download it from https://github.com/Tencent/rapidjson")
endif()

add_library(${PROJECT_LIB_NAME} ${PROJECT_HEADER_FILES} ${PROJECT_SOURCE_FILES})
set_target_properties(${PROJECT_LIB_NAME} PROPERTIES CXX_VISIBILITY_PRESET "hidden")
target_link_libraries(${PROJECT_LIB_NAME} PRIVATE Threads::Threads)

# Use generator expressions for include directories
target_include_directories(${PROJECT_LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        $<BUILD_INTERFACE:${RAPIDJSON_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include/cityflow>
)

# ============================================
# Installation configuration
# ============================================

include(GNUInstallDirs)

# Install the library
install(TARGETS ${PROJECT_LIB_NAME}
        EXPORT CityFlowTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers maintaining directory structure
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/engine
        ${CMAKE_CURRENT_LIST_DIR}/flow
        ${CMAKE_CURRENT_LIST_DIR}/roadnet
        ${CMAKE_CURRENT_LIST_DIR}/utility
        ${CMAKE_CURRENT_LIST_DIR}/vehicle
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cityflow
        FILES_MATCHING PATTERN "*.h"
)

# Install RapidJSON headers (they're needed by CityFlow headers)
install(DIRECTORY ${RAPIDJSON_INCLUDE_DIR}/rapidjson
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cityflow
        FILES_MATCHING PATTERN "*.h"
)

# Create and install CMake config files
install(EXPORT CityFlowTargets
        FILE CityFlowTargets.cmake
        NAMESPACE CityFlow::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CityFlowCpp
)

# Create config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/CityFlowCppConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/CityFlowCppConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CityFlowCpp
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/CityFlowCppConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CityFlowCpp
)