#pragma once

#include "common/singleton.h"
#include "logger/quantlogger.h"
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

namespace quanttrader {
namespace test {

class TestBase;

// Singleton
class TestFunctionMgr: public Singleton<TestFunctionMgr> {
public:
    bool register_test(const std::string_view name, std::function<std::shared_ptr<TestBase>(const std::string &name)> function);
    void run_all_tests();
    bool run_test(const std::string &name);

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<TestBase>(const std::string &name)>> test_functions_;
    quanttrader::log::LoggerPtr logger_ = quanttrader::log::get_common_rotation_logger("test", "test");
};

class TestBase {
public:
    TestBase(const std::string_view name): name_(name) {};
    virtual ~TestBase() {};

    virtual void run() = 0;

    template<typename T>
    static std::shared_ptr<TestBase> create(const std::string &name) {
        return std::shared_ptr<TestBase>(new T);
    }

protected:
    template<typename T>
    bool register_test() {
        TestFunctionMgr &mgr = TestFunctionMgr::instance();
        return mgr.register_test(name_, create<T>);
    }

private:
    std::string name_;
};

}
}