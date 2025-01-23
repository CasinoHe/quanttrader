#pragma

#include <string>
#include <any>
#include <memory>
#include <unordered_map>

namespace quanttrader {
namespace runner {

struct RunnerConfig {
    std::string runner_name;
    std::string strategy_name;
    std::unordered_map<std::string, std::any> addtionals;
};

class RunnerBase {
public:
    RunnerBase() = default;
    ~RunnerBase() = default;

    virtual void on_init() = 0;         // initialize the runner
    virtual void on_start() = 0;        // start the runner, there is data to process, all the data is prepared
    virtual void on_finished() = 0;     // finish the runner, prepare to exit
    virtual void on_tick() = 0;         // process the tick data
    virtual void on_bar() = 0;          // process the bar data
};

}
}