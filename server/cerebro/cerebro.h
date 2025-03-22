#pragma once

// Include all the Cerebro related headers
#include "cerebro_base.h"
#include "cerebro_factory.h"
#include "cerebro_manager.h"
#include "cerebro_util.h"
#include "cerebro_consts.h"
#include "backtest_cerebro.h"
#include "live_cerebro.h"

/**
 * @file cerebro.h
 * @brief Consolidated header for the Cerebro system
 * 
 * This header includes all the necessary components of the Cerebro system,
 * making it easier to use the Cerebro functionality in other parts of the application.
 * 
 * Usage example:
 * 
 * ```cpp
 * #include "cerebro/cerebro.h"
 * 
 * // Create parameters for a backtest
 * auto params = quanttrader::cerebro::CerebroUtil::create_default_params("backtest");
 * (*params)["initial_cash"] = 100000.0;
 * 
 * // Create a backtest cerebro
 * auto cerebro = quanttrader::cerebro::CerebroUtil::create_backtest("my_backtest", params);
 * 
 * // Add strategies, data providers, and set broker
 * cerebro->add_strategy(my_strategy);
 * cerebro->add_data(my_data_provider);
 * cerebro->set_broker(my_broker);
 * 
 * // Run the backtest
 * cerebro->run();
 * ```
 */

namespace quanttrader {
namespace cerebro {

// Function to register all available cerebro implementations
// This is called automatically when the library is loaded
extern int register_cerebros();

} // namespace cerebro
} // namespace quanttrader