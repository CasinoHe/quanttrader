// signalhandler.h
#pragma once
#include "EReaderOSSignal.h"
#include <functional>

namespace quanttrader {
namespace broker {

class SignalHandler : public EReaderOSSignal {
public:
    SignalHandler(unsigned long waitTimeout = INFINITE): EReaderOSSignal(waitTimeout) {}
    virtual ~SignalHandler() {}
};

}
}
