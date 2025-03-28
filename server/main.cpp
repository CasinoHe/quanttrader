#include "logger/quantlogger.h"
#include "config/lua_config_loader.h"
#include "boost/program_options.hpp"
#include "service/service_factory.h"
#include "service/service.h"

#ifdef QUANTTRADER_BUILD_TEST
#include "test/test_base.h"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>


namespace po = boost::program_options;
namespace qlog = quanttrader::log;

constexpr char STRATEGY_CONFIG_TABLE_NAME[] = "strategy_config";
constexpr char TEST_CONFIG_TABLE_NAME[] = "test_config";

int run_test_command(const std::string &config_path) {
#ifdef QUANTTRADER_BUILD_TEST
    if (config_path.empty()) {
        std::cerr << "Error: No configuration file specified for 'test'.\n";
        return EXIT_FAILURE;
    }
    
    std::cout << "Running test using configuration: " << config_path << "\n";
    // The test function should read the configuration file to determine which test to run
    auto &mgr = quanttrader::test::TestFunctionMgr::instance();
    // The actual test function should be specified in the config file
    auto config_loader = quanttrader::luascript::LuaConfigLoader(config_path);
    if (!config_loader.load_config()) {
        std::cerr << "Error: Failed to load configuration file: " << config_path << "\n";
        return EXIT_FAILURE;
    } else {
        qlog::Info("Running test using configuration: {}", config_path);
    }
    auto func_name = config_loader.get_string_value(TEST_CONFIG_TABLE_NAME, "function_name");
    return mgr.run_test(func_name);
#else
    std::cout << "Not build with test support. Please recompile with QUANTTRADER_BUILD_TEST defined." << std::endl;
    return EXIT_SUCCESS;
#endif
}

int run_strategy_command(const std::string &config_path) {
    if (config_path.empty()) {
        std::cerr << "Error: No configuration file specified for 'strategy'.\n";
        return EXIT_FAILURE;
    }
    
    auto config_loader = quanttrader::luascript::LuaConfigLoader(config_path);
    if (!config_loader.load_config()) {
        std::cerr << "Error: Failed to load configuration file: " << config_path << "\n";
        return EXIT_FAILURE;
    } else {
        qlog::Info("Running strategy using configuration: {}", config_path);
    }
    auto strategy_config_path = config_loader.get_string_value(STRATEGY_CONFIG_TABLE_NAME, "config_path");
    auto need_start_service = config_loader.get_string_value(STRATEGY_CONFIG_TABLE_NAME, "start_services");

    namespace qservice = quanttrader::service;
    // All strategy parameters are now in the config file
    auto service = qservice::ServiceFactory::instance()->createService(need_start_service, strategy_config_path);
    if (!service->prepare()) {
        std::cout << "Cannot prepare the service. Check log/service.log for more information." << std::endl;
        return EXIT_FAILURE;
    } else {
        service->run();
        return EXIT_SUCCESS;
    }
}

int main(int argc, const char* argv[]) {
    try {
        // init logger first
        qlog::QuantLogger::init();

        // Simplified options - only command and config
        po::options_description global_desc("QuantTrader options");
        global_desc.add_options()
            ("help,h", "Display help message")
            ("command", po::value<std::string>()->default_value("strategy"), "Command to execute, choose from [test, strategy], default is strategy")
            ("config,c", po::value<std::string>()->default_value("main_config.lua"), "Path to the configuration file. Default value is : main_config.lua");

        // Parse options
        po::variables_map global_vm;
        po::store(po::command_line_parser(argc, argv).options(global_desc).run(), global_vm);
        po::notify(global_vm);

        // Display help for global options
        if (global_vm.count("help")) {
            std::cout << global_desc << "\n";
            return EXIT_SUCCESS;
        }

        // Check for the command
        if (!global_vm.count("command")) {
            std::cerr << "Error: No command provided.\n";
            std::cout << global_desc << "\n";
            return EXIT_FAILURE;
        }

        // Check for the config
        if (!global_vm.count("config")) {
            std::cerr << "Error: No configuration file provided.\n";
            std::cout << global_desc << "\n";
            return EXIT_FAILURE;
        }

        // Check the configuration file path
        std::string config_path = global_vm["config"].as<std::string>();
        if (config_path.empty()) {
            std::cerr << "Error: Configuration file path is empty.\n";
            return EXIT_FAILURE;
        }
        if (!std::filesystem::exists(config_path)) {
            std::cerr << "Error: Configuration file does not exist: " << config_path << "\n";
            return EXIT_FAILURE;
        }

        // Process the command
        std::string command = global_vm["command"].as<std::string>();
        if (command == "test") {
            return run_test_command(config_path);
        } else if (command == "strategy") {
            return run_strategy_command(config_path);
        } else {
            std::cerr << "Error: Unknown command '" << command << "'.\n";
            std::cout << "Valid commands are: test, strategy\n";
            return EXIT_FAILURE;
        }
    } catch (const po::error &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
