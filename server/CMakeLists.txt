cmake_minimum_required(VERSION 3.22)

# disable FindBoost module removed warning
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

project(quanttrader)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the project root directory
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
get_filename_component(PROJECT_ROOT_DIR ${PROJECT_ROOT_DIR} ABSOLUTE)
set(SERVER_ROOT_DIR ${PROJECT_ROOT_DIR}/server)

# get all *.cpp and *.c files to SERVER_SOURCES
file(GLOB_RECURSE SERVER_SOURCES LIST_DIRECTORIES false ${SERVER_ROOT_DIR}/*.cpp)
file(GLOB_RECURSE C_SOURCES LIST_DIRECTORIES false ${SERVER_ROOT_DIR}/*.c)
list(APPEND SERVER_SOURCES ${C_SOURCES})
# Exclude files in the build directory and its subdirectories
list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/build/.*")
list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/strategy/slope_strategy/.*")
list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/strategy/strategy_base.cpp")
list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/strategy/strategy_factory.cpp")
list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/strategy/strategy_loader.cpp")
list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/strategy/broker_aware_strategy.cpp")

# -------------------------- build test code ----------------------------
# TODO: build test using python/lua script in runtime
option(BUILD_TEST "Build test" OFF)
if (BUILD_TEST)
    add_definitions(-DQUANTTRADER_BUILD_TEST)
else()
    list(FILTER SERVER_SOURCES EXCLUDE REGEX "${SERVER_ROOT_DIR}/test/.*")
endif()
# -------------------------- build test end ----------------------------

set(THIRDPARTY_DIR ${PROJECT_ROOT_DIR}/thirdparties)

# find all dependencies and add them to the project
# the dependencies is built by vcpkg or custom build
find_package(fmt 11.0.2 REQUIRED)
find_package(spdlog 1.15.0 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options algorithm stacktrace_basic)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

# find date library only on Apple platform
if (APPLE)
    find_package(date CONFIG REQUIRED)
endif()

# --------------------- build lua start ---------------------------
set(LUA_ROOT "${THIRDPARTY_DIR}/lua")
file(GLOB LUA_SOURCES LIST_DIRECTORIES false ${LUA_ROOT}/*.c)
list(REMOVE_ITEM LUA_SOURCES ${LUA_ROOT}/lua.c ${LUA_ROOT}/onelua.c)
include_directories(${LUA_ROOT})
# --------------------- build lua end ---------------------------

# --------------------- build ta-lib start ---------------------------
include(${SERVER_ROOT_DIR}/TALib.cmake)
# --------------------- build ta-lib end ---------------------------

# --------------------- build tws api start ---------------------------
set(TWS_ROOT "${THIRDPARTY_DIR}/tws/source/cppclient/client")
set(TWS_PROTO_ROOT "${TWS_ROOT}/protobufUnix")
file(GLOB TWS_SOURCES ${TWS_ROOT}/*.cpp)
file(GLOB TWS_HEADERS ${TWS_ROOT}/*.h)
file(GLOB TWS_PROTO_SOURCES ${TWS_PROTO_ROOT}/*.cc)
file(GLOB TWS_PROTO_HEADERS ${TWS_PROTO_ROOT}/*.h)
set(twsapi_VERSION 9.79.02)
# --------------------- build tws api end ---------------------------

# arrange the source files
source_group("lua" FILES ${LUA_SOURCES})
source_group("tws" FILES ${TWS_SOURCES} ${TWS_HEADERS})
source_group("server" FILES ${SERVER_SOURCES})

# Define common include directories
set(COMMON_INCLUDE_DIRS
    ${THIRDPARTY_DIR}/fmt/include
    ${THIRDPARTY_DIR}/spdlog/include
    ${SERVER_ROOT_DIR}
    ${SERVER_ROOT_DIR}/basic
    ${Boost_INCLUDE_DIRS}
    ${TWS_ROOT}
    ${TWS_PROTO_ROOT}
)

# core strategy library to be linked by executable and strategy plugins
add_library(strategy_core STATIC
    strategy/strategy_base.cpp
    strategy/strategy_factory.cpp
    strategy/strategy_loader.cpp
    basic/logger/quantlogger.cpp
    basic/config/lua_config_loader.cpp
    ${LUA_SOURCES}
)
set_target_properties(strategy_core PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(strategy_core PUBLIC ${COMMON_INCLUDE_DIRS})
target_link_libraries(strategy_core PUBLIC 
    fmt::fmt
    spdlog::spdlog
)

add_executable(${PROJECT_NAME} ${SERVER_SOURCES} ${LUA_SOURCES} ${TWS_SOURCES} ${TA_LIB_SOURCES} ${TWS_PROTO_SOURCES} ${TWS_PROTO_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})
get_filename_component(THIRD_PARTY_LIB_DIRS ${Boost_INCLUDE_DIRS}/../lib ABSOLUTE)
target_link_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_LIB_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
    strategy_core
    Boost::program_options
    Boost::algorithm
    Boost::stacktrace_basic
    intel_decimal128
    unofficial::concurrentqueue::concurrentqueue
    CURL::libcurl
    unofficial::sqlite3::sqlite3
    rapidjson
    protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE date::date date-tz)
endif()

if (WIN32)
    # target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MT")
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)
endif()

# Build strategy plugins
add_subdirectory(${PROJECT_ROOT_DIR}/strategies strategies_build)

