cmake_minimum_required(VERSION 3.22)
project(SlopeStrategyPlugin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Paths to quanttrader source
get_filename_component(SERVER_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(PROJECT_ROOT_DIR "${SERVER_ROOT_DIR}/..")
set(THIRDPARTY_DIR "${PROJECT_ROOT_DIR}/thirdparties")

# dependencies from quanttrader
find_package(fmt 11.0.2 REQUIRED)
find_package(spdlog 1.15.0 REQUIRED)

include("${SERVER_ROOT_DIR}/TALib.cmake")

# build core strategy library
add_library(strategy_core STATIC
    "${SERVER_ROOT_DIR}/strategy/strategy_base.cpp"
    "${SERVER_ROOT_DIR}/strategy/broker_aware_strategy.cpp"
)
set_target_properties(strategy_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(strategy_core PUBLIC "${SERVER_ROOT_DIR}")

# slope strategy plugin
add_library(SlopeStrategy SHARED
    slope_strategy.cpp
    slope_strategy_register.cpp
    ${TA_LIB_SOURCES}
)
set_target_properties(SlopeStrategy PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/strategies")

target_include_directories(SlopeStrategy PRIVATE
    "${SERVER_ROOT_DIR}"
    "${SERVER_ROOT_DIR}/basic"
)

target_link_libraries(SlopeStrategy PRIVATE
    strategy_core
    fmt::fmt
    spdlog::spdlog
)
