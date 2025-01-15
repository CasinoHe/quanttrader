// signalhandler.h
#pragma once
#include "EReaderOSSignal.h"
#include <functional>

class SignalHandler : public EReaderOSSignal {
public:
    SignalHandler(unsigned long waitTimeout = INFINITE): EReaderOSSignal(waitTimeout) {}
    virtual ~SignalHandler() {}
};
