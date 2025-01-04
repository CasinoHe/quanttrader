cmake_minimum_required(VERSION 3.22)

project(quanttrader)

# Set the project root directory
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
get_filename_component(PROJECT_ROOT_DIR ${PROJECT_ROOT_DIR} ABSOLUTE)
set(SERVER_ROOT_DIR ${PROJECT_ROOT_DIR}/server)

# get all *.cpp and *.c files to SERVER_SOURCES
file(GLOB_RECURSE SERVER_SOURCES LIST_DIRECTORIES false ${SERVER_ROOT_DIR}/*.cpp)
file(GLOB_RECURSE C_SOURCES LIST_DIRECTORIES false ${SERVER_ROOT_DIR}/*.c)
list(APPEND SERVER_SOURCES ${C_SOURCES})

message(STATUS "SERVER_SOURCES: ${SERVER_SOURCES}")

# build fmt library
add_subdirectory(${PROJECT_ROOT_DIR}/thirdparties/fmt ${CMAKE_BINARY_DIR}/fmt)

# add spdlog based on fmt library
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(${PROJECT_ROOT_DIR}/thirdparties/spdlog ${CMAKE_BINARY_DIR}/spdlog)
target_include_directories(spdlog INTERFACE ${PROJECT_ROOT_DIR}/thirdparties/fmt/include)

add_executable(${PROJECT_NAME} ${SERVER_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT_DIR}/thirdparties/fmt/include
                                                  ${PROJECT_ROOT_DIR}/thirdparties/spdlog/include
                                                  ${SERVER_ROOT_DIR}
                                                  )
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt spdlog::spdlog)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)