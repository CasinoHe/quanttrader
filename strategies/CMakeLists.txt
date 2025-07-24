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
if(NOT TARGET strategy_core)
    if(EXISTS "${QUANTTRADER_SERVER_ROOT}/strategy/strategy_base.cpp")
        add_library(strategy_core STATIC
            "${QUANTTRADER_SERVER_ROOT}/strategy/strategy_base.cpp"
            "${QUANTTRADER_SERVER_ROOT}/strategy/broker_aware_strategy.cpp"
        )
        set_target_properties(strategy_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
        target_include_directories(strategy_core PUBLIC "${QUANTTRADER_SERVER_ROOT}")
        
        # Link dependencies if available
        if(TARGET fmt::fmt)
            target_link_libraries(strategy_core PUBLIC fmt::fmt)
        endif()
        if(TARGET spdlog::spdlog)
            target_link_libraries(strategy_core PUBLIC spdlog::spdlog)
        endif()
    else()
        message(FATAL_ERROR "strategy_core source files not found in ${QUANTTRADER_SERVER_ROOT}/strategy/")
    endif()
endif()

# Common strategy utilities
add_subdirectory(common)

# Individual strategies
add_subdirectory(slope_strategy)

# Output directory for all strategy plugins
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/plugins")
