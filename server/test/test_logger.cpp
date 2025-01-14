#include "logger/quantlogger.h"
#include "test/test_base.h"

namespace qlog = quanttrader::log;

// TODO: change to build and run it under python/lua script
// int main(int argc, const char **argv) {
namespace quanttrader {
namespace test {

class TestLogger : public TestBase {
public:
    TestLogger(): TestBase("TestLogger") {
        register_test<TestLogger>();
    }

    virtual void run() override {
        auto test_file_logger = qlog::g_logger_mgr.get_file_logger("test", "test");
        auto console_logger = qlog::g_logger_mgr.get_console_logger("console");
        auto test_file_and_console_logger = qlog::g_logger_mgr.get_file_and_console_logger("test_console", "test_console");

        test_file_logger->info("Test file logger info");
        test_file_logger->warn("Test file logger warn");

        console_logger->info("Test console logger info");
        console_logger->warn("Test console logger warn");

        test_file_and_console_logger->info("Test file and console logger info");
        test_file_and_console_logger->warn("Test file and console logger warn");

        qlog::g_logger->info("Global logger info");
        qlog::g_logger->warn("Global logger warn");
    }
};

// dummy object to register test
static TestLogger test_logger;

}
}