cmake_minimum_required(VERSION 3.22)

project(quanttrader)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
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

set(THIRDPARTY_DIR ${PROJECT_ROOT_DIR}/thirdparties)

# build fmt library
add_subdirectory(${THIRDPARTY_DIR}/fmt ${CMAKE_BINARY_DIR}/fmt)

# add spdlog based on fmt library
set(SPDLOG_FMT_EXTERNAL ON)
add_subdirectory(${THIRDPARTY_DIR}/spdlog ${CMAKE_BINARY_DIR}/spdlog)
target_include_directories(spdlog INTERFACE ${THIRDPARTY_DIR}/fmt/include)

# --------------------- add boost and build boost ---------------------------
set(BOOST_ROOT "${THIRDPARTY_DIR}/boost")
set(BOOST_VERSION 1.86.0)

# Define platform-specific commands for bootstrapping
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(BOOST_BOOTSTRAP_COMMAND "${BOOST_ROOT}/bootstrap.bat")
    set(BOOST_BUILD_COMMAND "b2")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS: Use bootstrap.sh
    set(BOOST_BOOTSTRAP_COMMAND "${BOOST_ROOT}/bootstrap.sh")
    set(BOOST_BUILD_COMMAND "./b2")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BOOST_BOOTSTRAP_COMMAND "${BOOST_ROOT}/bootstrap.sh")
    set(BOOST_BUILD_COMMAND "./b2")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

include(ExternalProject)
ExternalProject_Add(
    boost_build
    SOURCE_DIR ${BOOST_ROOT}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ${BOOST_BOOTSTRAP_COMMAND}
    BUILD_COMMAND ${BOOST_BUILD_COMMAND} headers && ${BOOST_BUILD_COMMAND} --with-system variant=release link=static runtime-link=static threading=multi stage
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE TRUE
)

# Locate Boost after building
add_custom_target(boost_find ALL
    COMMENT "Finding Boost libraries after building"
    DEPENDS boost_build
)

# Find Boost in the main CMakeLists.txt after boost_build
if(TARGET boost_find)
    if (POLICY CMP0144)
        cmake_policy(SET CMP0144 NEW)
    endif()
    find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS system PATHS ${BOOST_ROOT}/stage/lib/cmake/Boost-${BOOST_VERSION} NO_CMAKE_SYSTEM_PATH)

    if(Boost_FOUND)
        message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "Boost not found after building!")
    endif()
endif()

# --------------------- add boost and build boost end ---------------------------

add_executable(${PROJECT_NAME} ${SERVER_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/fmt/include
                                                  ${THIRDPARTY_DIR}/spdlog/include
                                                  ${SERVER_ROOT_DIR}
                                                  )
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt spdlog::spdlog ${BOOST_LIBRARIES})
