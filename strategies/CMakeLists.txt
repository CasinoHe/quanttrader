cmake_minimum_required(VERSION 3.22)
project(QuantTraderStrategies)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Path to quanttrader server (can be overridden)
# For submodule use, this should be set by the parent project
if(NOT DEFINED QUANTTRADER_SERVER_ROOT)
    # Default assumption: strategies is a subdirectory of quanttrader
    get_filename_component(QUANTTRADER_SERVER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../server" ABSOLUTE)
endif()

if(NOT EXISTS "${QUANTTRADER_SERVER_ROOT}")
    message(FATAL_ERROR "QuantTrader server root not found at: ${QUANTTRADER_SERVER_ROOT}")
endif()

set(PROJECT_ROOT_DIR "${QUANTTRADER_SERVER_ROOT}/..")
set(THIRDPARTY_DIR "${PROJECT_ROOT_DIR}/thirdparties")

message(STATUS "QuantTrader Server Root: ${QUANTTRADER_SERVER_ROOT}")
message(STATUS "QuantTrader Strategies can be built independently or as part of the main project")

# Add global include directories for all strategy targets
include_directories(
    "${QUANTTRADER_SERVER_ROOT}"
    "${QUANTTRADER_SERVER_ROOT}/basic"
    "${THIRDPARTY_DIR}/fmt/include"
    "${THIRDPARTY_DIR}/spdlog/include"
    "${THIRDPARTY_DIR}/tws/source/cppclient/client"
)

# Find dependencies
# Try to find dependencies that might be installed system-wide or in vcpkg
find_package(fmt 11.0.2 QUIET)
find_package(spdlog 1.15.0 QUIET)

if(NOT fmt_FOUND OR NOT spdlog_FOUND)
    message(STATUS "Some dependencies not found, assuming they are part of QuantTrader build")
    # If building as part of QuantTrader, dependencies should already be configured
endif()

# Include TA-Lib build configuration
if(EXISTS "${QUANTTRADER_SERVER_ROOT}/TALib.cmake")
    include("${QUANTTRADER_SERVER_ROOT}/TALib.cmake")
else()
    message(WARNING "TALib.cmake not found, TA-Lib functionality may not be available")
endif()

# Build strategy core library (if not already built)
# The strategy_core library is now built in the main server CMakeLists.txt
# This section is kept for standalone builds of strategies only
if(NOT TARGET strategy_core)
    message(FATAL_ERROR "strategy_core target not found. This should be built by the main server project.")
endif()

# Common strategy utilities
add_subdirectory(common)

# Individual strategies
add_subdirectory(slope_strategy)
add_subdirectory(broker_aware_strategy)

# Output directory for all strategy plugins
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/plugins")
