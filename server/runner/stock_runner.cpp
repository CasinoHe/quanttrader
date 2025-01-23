#include "stock_runner.h"

namespace quanttrader {
namespace runner {

void StockRunner::on_init() {
    // initialize the runner
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



}
}