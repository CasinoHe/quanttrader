cmake_minimum_required(VERSION 3.22)

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
message(STATUS "SERVER_SOURCES: ${SERVER_SOURCES}")

set(THIRDPARTY_DIR ${PROJECT_ROOT_DIR}/thirdparties)

# disable FindBoost module removed warning
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()
# find all dependencies and add them to the project
# the dependencies is built by vcpkg or custom build
find_package(fmt 11.0.2 REQUIRED)
find_package(spdlog 1.15.0 REQUIRED)
find_package(Boost 1.86.0 REQUIRED COMPONENTS program_options)

# --------------------- build lua start ---------------------------
set(LUA_ROOT "${THIRDPARTY_DIR}/lua")
file(GLOB LUA_SOURCES LIST_DIRECTORIES false ${LUA_ROOT}/*.c)
list(REMOVE_ITEM LUA_SOURCES ${LUA_ROOT}/lua.c ${LUA_ROOT}/onelua.c)
include_directories(${LUA_ROOT})
# --------------------- build lua end ---------------------------

# --------------------- build tws api start ---------------------------
set(TWS_ROOT "${THIRDPARTY_DIR}/tws")
file(GLOB TWS_SOURCES ${TWS_ROOT}/*.cpp)
file(GLOB TWS_HEADERS ${TWS_ROOT}/*.h)
set(twsapi_VERSION 9.79.02)
add_library(twsapi STATIC ${TWS_SOURCES})
if(WIN32)
    target_link_libraries(twsapi PUBLIC ws2_32)
endif()
if(MSVC)
    target_compile_options(twsapi PRIVATE /W3)
else()
    target_compile_options(twsapi PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)
endif()
set_target_properties(twsapi
    PROPERTIES
    PREFIX "lib"
    DEBUG_POSTFIX "d"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    PUBLIC_HEADER "${HEADERS}"
)
target_link_libraries(twsapi PRIVATE intel_decimal128)
# --------------------- build tws api end ---------------------------

add_executable(${PROJECT_NAME} ${SERVER_SOURCES} ${LUA_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/fmt/include
                                                  ${THIRDPARTY_DIR}/spdlog/include
                                                  ${SERVER_ROOT_DIR}
                                                  ${Boost_INCLUDE_DIRS}
                                                  )
target_link_directories(${PROJECT_NAME} PRIVATE ${Boost_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt spdlog::spdlog Boost::program_options twsapi)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/MT")
endif()
