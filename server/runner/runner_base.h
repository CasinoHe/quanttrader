#pragma

#include "runner_types.h"
#include "logger/quantlogger.h"
#include "service/service_consts.h"

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

        try {
            auto name = params_->find(STRATEGY_NAME_VARIABLE);
            if (name != params_->end()) {
                strategy_name_ = std::any_cast<std::string>(name->second);
            }
        } catch (std::exception &e) {
            logger_->error("Cannot find the strategy name in the runner params. error: {}", e.what());
            return;
        }

        logger_->info("Create runner: {}, strategy: {}", runner_name_, strategy_name_);
    };

    virtual ~Runner() {
        stop();
    }

    std::string get_runner_name() const noexcept { return runner_name_; }
    std::string get_strategy_name() const noexcept { return strategy_name_; }

    virtual void on_tick() = 0;         // process the tick data
    virtual void on_bar() = 0;          // process the bar data
    virtual void on_trade() = 0;        // process the trade data

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
            while (!initied_flag_.load()) {
                if (!on_init()) {
                    logger_->info("Initialization failed. Runner {} stoped without init.");
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (stop_flag_.load()) {
                    logger_->info("Runner {} stoped without init.");
                    return;
                }
            }

            on_start();

            while (!stop_flag_.load()) {
                run_frame();
            }

            on_finished();
        };

        runner_thread_ = std::make_shared<std::thread>(run_func);
        logger_->info("Runner {} started.", runner_name_);
    }

    // means that the thread is ending, and can join, and join will exit shortly
    // but it not means that thread is already ended, it maybe cleaning the register, caution
    bool is_ending() {
        return ended_flag_.load();
    }

protected:
    // initialize the runner, it runs multiple times
    virtual bool on_init() {
        initied_flag_.store(true);
        return true;
    }

    virtual void on_start() = 0;        // start the runner, there is data to process, all the data is prepared

    // run a frame
    virtual void run_frame() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    virtual void on_finished() {
        ended_flag_.store(true);
    };

    virtual void on_history() = 0;      // when historical data loaded, calculate all historical data

    std::atomic<bool> ended_flag_ {false};
    std::atomic<bool> initied_flag_ {false};
    RunnerParamsType params_ {nullptr};
    std::string runner_name_ {""};
    std::string strategy_name_ {""};
    quanttrader::log::LoggerPtr logger_ {nullptr};

private:
    std::atomic<bool> stop_flag_ {false};
    std::shared_ptr<std::thread> runner_thread_ {nullptr};
};

}
}