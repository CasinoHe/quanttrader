#include "service/strategy_service.h"
#include "broker/twsclient.h"

namespace quanttrader {
namespace service {

void StrategyService::run() {
    broker::TwsClient client("20.161.50.141", 3762, 9090);
    client.connect();
}

}
}