set(STRATEGY_LIB SlopeStrategy)

add_library(${STRATEGY_LIB} SHARED
    slope_strategy.cpp
    slope_strategy_register.cpp
    ${TA_LIB_SOURCES}
)

set_target_properties(${STRATEGY_LIB} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/strategies
)

target_include_directories(${STRATEGY_LIB} PRIVATE
    ${SERVER_ROOT_DIR}
    ${SERVER_ROOT_DIR}/basic
)

target_link_libraries(${STRATEGY_LIB} PRIVATE
    strategy_core
    fmt::fmt
    spdlog::spdlog
)
