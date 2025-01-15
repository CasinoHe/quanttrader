#pragma once

#include <memory>

namespace quanttrader {

template <typename T>
class Singleton {
public:
    template<typename... Args>
    static std::shared_ptr<T> instance(Args&&... args) {
        static std::shared_ptr<T> instance(new T(std::forward<Args>(args)...), [](T* ptr) { delete ptr; });
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};
}