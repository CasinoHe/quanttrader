#pragma once

#include "service.h"
#include <string>

namespace quanttrader {
namespace service {

class StrategyService : public ServiceBase<StrategyService> {
public:
    void run();

private:
    friend class Singleton<StrategyService>;
    StrategyService() = default;
    ~StrategyService() = default;
};

}
}