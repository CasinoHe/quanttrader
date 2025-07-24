# Slope Strategy Plugin
add_library(SlopeStrategy SHARED
    slope_strategy.cpp
    slope_strategy_register.cpp
    ${TA_LIB_SOURCES}
)

# Set output directory and properties
set_target_properties(SlopeStrategy PROPERTIES
    OUTPUT_NAME "slope_strategy"
    PREFIX ""
    SUFFIX ".so"
)

target_include_directories(SlopeStrategy PRIVATE
    "${QUANTTRADER_SERVER_ROOT}"
    "${QUANTTRADER_SERVER_ROOT}/basic"
)

# Link to strategy_core and common utilities
target_link_libraries(SlopeStrategy PRIVATE
    strategy_core
    strategy_common
)

# Link to dependencies if they are available
if(TARGET fmt::fmt)
    target_link_libraries(SlopeStrategy PRIVATE fmt::fmt)
endif()

if(TARGET spdlog::spdlog)
    target_link_libraries(SlopeStrategy PRIVATE spdlog::spdlog)
endif()
