#include "stock_trade_service.h"
#include "service_factory.h"
#include "service/service.h"

namespace quanttrader {
namespace service {

// Function to register the StockTradeService with the factory
bool registerStockTradeService() {
    auto creatorFunc = [](const std::string& configPath) -> std::shared_ptr<ServiceInterface> {
        auto service = StockTradeService::instance(configPath);
        return std::static_pointer_cast<ServiceInterface>(service);
    };
    
    return ServiceFactory::instance().registerService("stock_trade", creatorFunc);
}

// Automatic registration
namespace {
    // This will register the StockTradeService when the library is loaded
    static bool registered = registerStockTradeService();
}

} // namespace service
} // namespace quanttrader
