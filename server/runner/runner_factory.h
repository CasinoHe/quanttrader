#pragma once

#include "runner_types.h"
#include <common/singleton.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <atomic>

namespace quanttrader {
namespace runner {

class Runner;

class RunnerFactory : public Singleton<RunnerFactory> {
public:
    std::shared_ptr<Runner> create_runner(const std::string &runner_name, RunnerParamsType params) {
        if (!has_runner(runner_name)) {
            return nullptr;
        }
        return runner_creators_[runner_name](params);
    }

    bool has_runner(const std::string &runner_name) {
        return runner_creators_.find(runner_name) != runner_creators_.end();
    }

    bool register_runner(const std::string &runner_name, RunnerCreatorFuncType creator) {
        if (runner_creators_.find(runner_name) != runner_creators_.end()) {
            return false;
        }
        runner_creators_[runner_name] = creator;
        return true;
    }

    bool replace_runner(const std::string &runner_name, RunnerCreatorFuncType creator) {
        if (runner_creators_.find(runner_name) == runner_creators_.end()) {
            return false;
        }
        runner_creators_[runner_name] = creator;
        return true;
    }

    bool remove_runner(const std::string &runner_name) {
        if (runner_creators_.find(runner_name) == runner_creators_.end()) {
            return false;
        }
        runner_creators_.erase(runner_name);
        return true;
    }

private:
    friend class Singleton<RunnerFactory>;
    RunnerFactory() = default;
    ~RunnerFactory() = default;

    std::unordered_map<std::string, RunnerCreatorFuncType> runner_creators_;
};

}
}