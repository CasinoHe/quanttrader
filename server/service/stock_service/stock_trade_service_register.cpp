#include "stock_trade_service.h"
#include "service/service_factory.h"
#include "service/service.h"

namespace quanttrader {
namespace service {

// Function to register the StockTradeService with the factory
bool registerStockTradeService() {
    auto creatorFunc = [](const std::string& configPath) -> std::shared_ptr<ServiceBase> {
        auto service = StockTradeService::instance(configPath);
        return std::static_pointer_cast<ServiceBase>(service);
    };
    
    return ServiceFactory::instance().registerService(STOCK_TRADE_SERVICE_NAME, creatorFunc);
}

// Automatic registration
namespace {
    // This will register the StockTradeService when the library is loaded
    static bool registered = registerStockTradeService();
}

} // namespace service
} // namespace quanttrader
