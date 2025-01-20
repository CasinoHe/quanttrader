set(TA_LIB_ROOT_DIR "${THIRDPARTY_DIR}/ta-lib")

set(TA_LIB_VERSION_MAJOR 0)
set(TA_LIB_VERSION_MINOR 6)
set(TA_LIB_VERSION_PATCH 4)

set(TA_LIB_VERSION "${TA_LIB_VERSION_MAJOR}.${TA_LIB_VERSION_MINOR}.${TA_LIB_VERSION_PATCH}")

# Default to Release config
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

# Source used in all end-user libraries and most TA-Lib executable.
file(GLOB TA_LIB_COMMON_SOURCES "${TA_LIB_ROOT_DIR}/src/ta_common/*.c")

# TA-Lib specify all its TA functions signature (and meta information
# such as the "group" it belongs to) using an "interface definition language"
# written in C.
file(GLOB_RECURSE TA_LIB_ABSTRACT_SOURCES "${TA_LIB_ROOT_DIR}/src/ta_abstract/*.c")

###############################
# static libraries #
###############################
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckTypeSize)
include(CheckSymbolExists)
check_include_files(float.h HAVE_FLOAT_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(limits.h HAVE_LIMITS_H)
check_include_files(locale.h HAVE_LOCALE_H)
check_include_files(stddef.h HAVE_STDDEF_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(wchar.h HAVE_WCHAR_H)
check_include_files(wctype.h HAVE_WCTYPE_H)
check_function_exists(floor HAVE_FLOOR)
check_function_exists(isascii HAVE_ISASCII)
check_function_exists(localeconv HAVE_LOCALECONV)
check_function_exists(mblen HAVE_MBLEN)
check_function_exists(memmove HAVE_MEMMOVE)
check_function_exists(memset HAVE_MEMSET)
check_function_exists(modf HAVE_MODF)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)
check_function_exists(strcasecmp HAVE_STRCASECMP)
check_function_exists(strchr HAVE_STRCHR)
check_function_exists(strerror HAVE_STRERROR)
check_function_exists(strncasecmp HAVE_STRNCASECMP)
check_function_exists(strrchr HAVE_STRRCHR)
check_function_exists(strstr HAVE_STRSTR)
check_function_exists(strtol HAVE_STRTOL)
check_function_exists(strtoul HAVE_STRTOUL)
check_function_exists(strcoll HAVE_STRCOLL)
check_function_exists(strftime HAVE_STRFTIME)
check_function_exists(vprintf HAVE_VPRINTF)
check_type_size(ptrdiff_t SIZEOF_PTRDIFF_T)
if(HAVE_SIZEOF_PTRDIFF_T)
	set(HAVE_PTRDIFF_T 1)
else()
	set(HAVE_PTRDIFF_T 0)
endif()
check_type_size(size_t SIZEOF_SIZE_T)
if(HAVE_SIZEOF_SIZE_T)
	set(HAVE_SIZE_T 1)
else()
	set(HAVE_SIZE_T 0)
	set(size_t "unsigned")
endif()
check_symbol_exists("struct tm" "sys/time.h" TM_IN_SYS_TIME)
configure_file("${TA_LIB_ROOT_DIR}/include/ta_config.h.cmake" "${TA_LIB_ROOT_DIR}/include/ta_config.h")

include_directories(
	"${TA_LIB_ROOT_DIR}/include"
	"${TA_LIB_ROOT_DIR}/src/ta_abstract"
	"${TA_LIB_ROOT_DIR}/src/ta_abstract/frames"
	"${TA_LIB_ROOT_DIR}/src/ta_common"
	"${TA_LIB_ROOT_DIR}/src/ta_func"
)

# TA-Lib functions implementation.
file(GLOB TA_LIB_FUNC_SOURCES ${TA_LIB_ROOT_DIR}/src/ta_func/*.c)

set(TA_LIB_SOURCES ${TA_LIB_COMMON_SOURCES} ${TA_LIB_ABSTRACT_SOURCES} ${TA_LIB_FUNC_SOURCES})
