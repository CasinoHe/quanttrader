#include "stock_runner.h"
#include <thread>

namespace quanttrader {
namespace runner {

void StockRunner::on_init() {
    // initialize the runner
    Runner::on_init();
}

void StockRunner::on_start() {
    // start the runner, there is data to process, all the data is prepared
}

void StockRunner::on_finished() {
    // finish the runner, prepare to exit
}

void StockRunner::on_tick() {
    // process the tick data
}

void StockRunner::on_bar() {
    // process the bar data
}

void StockRunner::run_frame() {
    // run a frame
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}
}