#include "test_base.h"
#include <format>


namespace quanttrader {
namespace test {


bool TestFunctionMgr::register_test(const std::string_view name, std::function<std::shared_ptr<TestBase>(const std::string &name)> function) {
    if (test_functions_.find(std::string(name)) != test_functions_.end()) {
        logger_->error("Test with name {} already exists", name);
        return false;
    }

    test_functions_[std::string(name)] = function;
    logger_->info("Registered test with name {}", name);
    return true;
}

bool TestFunctionMgr::run_test(const std::string &name) {
    auto it = test_functions_.find(name);
    if (it == test_functions_.end()) {
        logger_->error("Test with name {} not found", name);
        return false;
    }

    std::shared_ptr<TestBase> test = it->second(name);
    test->run();
    return true;
}

}
}