#pragma

#include "runner_types.h"
#include "logger/quantlogger.h"

#include <string>
#include <any>
#include <memory>
#include <unordered_map>
#include <atomic>
#include <thread>

namespace quanttrader {
namespace runner {

class RunnerFactory;

class Runner {
public:
    Runner(const std::string_view runner_name, RunnerParamsType params): runner_name_(runner_name), params_(params) {
        logger_ = quanttrader::log::get_common_rotation_logger(runner_name_, "runner", false);
        logger_->info("Create runner: {}", runner_name_);

        run(); 
    };

    virtual ~Runner() {
        stop();
    }

    virtual void on_tick() = 0;         // process the tick data
    virtual void on_bar() = 0;          // process the bar data

    void stop() {
        stop_flag_.store(true);
        if (runner_thread_) {
            if (runner_thread_->joinable()) {
                runner_thread_->join();
            }
            runner_thread_.reset();
        }
    }; // stop the runner

    void run() {
        auto run_func = [this]() {
            on_init();

            while (!initied_flag_.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            on_start();

            while (!stop_flag_.load()) {
                run_frame();
            }

            on_finished();
        };

        runner_thread_ = std::make_shared<std::thread>(run_func);
    }

protected:
    virtual void on_init() = 0;         // initialize the runner
    virtual void on_start() = 0;        // start the runner, there is data to process, all the data is prepared

    // run a frame
    virtual void run_frame() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    virtual void on_finished() {
        ended_flag_.store(true);
    };

    std::atomic<bool> ended_flag_ {false};
    std::atomic<bool> initied_flag_ {false};
    RunnerParamsType params_ {nullptr};
    std::string runner_name_ {""};
    quanttrader::log::LoggerPtr logger_ {nullptr};

private:
    std::atomic<bool> stop_flag_ {false};
    std::shared_ptr<std::thread> runner_thread_ {nullptr};
};

}
}