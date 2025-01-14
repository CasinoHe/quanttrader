// signalhandler.h
#pragma once
#include "EReaderSignal.h"
#include <functional>

class SignalHandler : public EReaderSignal {
public:
    void set_on_signal(const std::function<void()> &handler) {
        on_signal_ = handler;
    }

    void handle_signal() {
        if (on_signal_) {
            on_signal_();
        }
    }

    virtual void issueSignal() override {};
    virtual void waitForSignal() override {};

private:
    std::function<void()> on_signal_;
};
