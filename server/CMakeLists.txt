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
find_package(Boost 1.86.0 REQUIRED COMPONENTS program_options algorithm)
find_package(unofficial-concurrentqueue CONFIG REQUIRED)

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
set(TWS_ROOT "${THIRDPARTY_DIR}/tws")
file(GLOB TWS_SOURCES ${TWS_ROOT}/*.cpp)
file(GLOB TWS_HEADERS ${TWS_ROOT}/*.h)
set(twsapi_VERSION 9.79.02)
# --------------------- build tws api end ---------------------------

# arrange the source files
source_group("lua" FILES ${LUA_SOURCES})
source_group("tws" FILES ${TWS_SOURCES} ${TWS_HEADERS})
source_group("server" FILES ${SERVER_SOURCES})

add_executable(${PROJECT_NAME} ${SERVER_SOURCES} ${LUA_SOURCES} ${TWS_SOURCES} ${TA_LIB_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/fmt/include
                                                  ${THIRDPARTY_DIR}/spdlog/include
                                                  ${SERVER_ROOT_DIR}
                                                  ${SERVER_ROOT_DIR}/basic
                                                  ${Boost_INCLUDE_DIRS}
                                                  ${TWS_ROOT}
                                                  )
get_filename_component(THIRD_PARTY_LIB_DIRS ${Boost_INCLUDE_DIRS}/../lib ABSOLUTE)
target_link_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_LIB_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt 
                                spdlog::spdlog
                                Boost::program_options
                                Boost::algorithm
                                intel_decimal128
                                unofficial::concurrentqueue::concurrentqueue
                                )

if (WIN32)
    # target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MT")
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)
endif()
