#include "logger/quantlogger.h"
#include "boost/program_options.hpp"
#include "service/service_factory.h"
#include "service/stock_trade_service.h"

#ifdef QUANTTRADER_BUILD_TEST
#include "test/test_base.h"
#endif

#include <iostream>
#include <vector>
#include <string>


namespace po = boost::program_options;
namespace qlog = quanttrader::log;

int parse_test_command(const std::vector<std::string> &subargs) {
    po::options_description add_desc("Options for 'test' command");
    add_desc.add_options()
        ("help,h", "Display help message")
        ("function", po::value<std::string>(), "What function to test");

    po::variables_map test_vm;
    po::store(po::command_line_parser(subargs).options(add_desc).run(), test_vm);
    po::notify(test_vm);

    if (test_vm.count("help")) {
        std::cout << add_desc << "\n";
        return EXIT_SUCCESS;
    }

    if (test_vm.count("function")) {
#ifdef QUANTTRADER_BUILD_TEST
        std::string name = test_vm["function"].as<std::string>();
        auto &mgr = quanttrader::test::TestFunctionMgr::instance();
        return mgr.run_test(name);
#else
        std::cout << "Not build with test support. Please recompile with QUANTTRADER_BUILD_TEST defined." << std::endl;
        return EXIT_SUCCESS;
#endif
    } else {
        std::cerr << "Error: No function specified for 'test'.\n";
        std::cout << add_desc << "\n";
        return 1;
    }

    return EXIT_SUCCESS;
}


int parse_strategy_command(const std::vector<std::string> &subargs) {
    po::options_description add_desc("Options for 'strategy' command");
    add_desc.add_options()
        ("help,h", "Display help message")
        ("name", po::value<std::string>(), "The name of strategy to run")
        ("config,c", po::value<std::string>(), "The configuration file for the strategy")
        ("loglevel", po::value<std::string>()->default_value("info"), "The log level for strategy");

    po::variables_map strategy_vm;
    po::store(po::command_line_parser(subargs).options(add_desc).run(), strategy_vm);
    po::notify(strategy_vm);

    if (strategy_vm.count("help")) {
        std::cout << add_desc << "\n";
        return EXIT_SUCCESS;
    }

    if (strategy_vm.count("loglevel")) {
        std::string loglevel = strategy_vm["loglevel"].as<std::string>();
        if (!qlog::QuantLogger::set_default_log_level(loglevel)) {
            std::cerr << "Error: Invalid log level specified for 'strategy'.\n";
            std::cout << add_desc << "\n";
            return EXIT_FAILURE;
        }
    }

    if (strategy_vm.count("name")) {
        namespace qservice = quanttrader::service;

        if (!strategy_vm.count("config")) {
            std::cerr << "Error: No configuration file specified for 'strategy'.\n";
            std::cout << add_desc << "\n";
            return EXIT_FAILURE;
        }
        std::string name = strategy_vm["name"].as<std::string>();
        std::string config_path = strategy_vm["config"].as<std::string>();
        std::cout << "Prepare to run strategy: " << name << "\n";
        auto service = qservice::ServiceFactory::get_service<qservice::StockTradeService>(config_path);
        if (!service->prepare()) {
            std::cout << "Cannot prepare the service. Check log/service.log for more information." << std::endl;
        } else {
            service->run();
        }
        return EXIT_SUCCESS;
    } else {
        std::cerr << "Error: No strategy specified for 'strategy'.\n";
        std::cout << add_desc << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


int main(int argc, const char* argv[]) {
    try {
        // init logger first
        qlog::QuantLogger::init();

        // Top-level options
        po::options_description global_desc("QuantTrader options");
        global_desc.add_options()
            ("help,h", "Display help message")
            ("command", po::value<std::string>(), "Command to execute, choose from [test, strategy]")
            ("subargs", po::value<std::vector<std::string>>()->multitoken(), "Arguments for command, forexample: quanttrader --command strategy --subargs name=a config=b.lua");

        // Parse top-level options
        po::variables_map global_vm;
        po::parsed_options parsed = po::command_line_parser(argc, argv)
                                        .options(global_desc)
                                        .allow_unregistered()
                                        .run();
        po::store(parsed, global_vm);
        po::notify(global_vm);

        // Display help for global options
        if (global_vm.count("help")) {
            std::cout << global_desc << "\n";
            return 0;
        }

        // Check for the command
        if (!global_vm.count("command")) {
            std::cerr << "Error: No command provided.\n";
            std::cout << global_desc << "\n";
            return 1;
        }

        std::string command = global_vm["command"].as<std::string>();
        std::vector<std::string> subargs =
            global_vm.count("subargs") ? global_vm["subargs"].as<std::vector<std::string>>() : std::vector<std::string>();

        // add "--" prefix to subargs for boost::program_options parser
        for (int i = 0; i < subargs.size(); i++) {
            if (!subargs[i].starts_with("--")) {
                subargs[i] = "--" + subargs[i];
            }
        }

        // Sub-command-specific options
        if (command == "test") {
            return parse_test_command(subargs);
        } else if (command == "strategy") {
            return parse_strategy_command(subargs);
        } else {
            std::cerr << "Error: Unknown command '" << command << "'.\n";
            return 1;
        }
    } catch (const po::error &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return EXIT_SUCCESS;
}
