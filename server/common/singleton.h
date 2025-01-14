#pragma once

namespace quanttrader {

template <typename T>
class Singleton {
public:
    static T& instance() {
        static T instance;
        return instance;
    }

protected:
    constexpr Singleton() = default;
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};
}