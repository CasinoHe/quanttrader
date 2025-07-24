# Broker Aware Strategy Plugin
add_library(BrokerAwareStrategy SHARED
    broker_aware_strategy.cpp
    broker_aware_strategy_register.cpp
)

# Set output directory and properties
set_target_properties(BrokerAwareStrategy PROPERTIES
    OUTPUT_NAME "broker_aware_strategy"
    PREFIX ""
    SUFFIX ".so"
)

target_include_directories(BrokerAwareStrategy PRIVATE
    "${QUANTTRADER_SERVER_ROOT}"
    "${QUANTTRADER_SERVER_ROOT}/basic"
)

# Link to strategy_core and common utilities
target_link_libraries(BrokerAwareStrategy PRIVATE
    strategy_core
    strategy_common
)
