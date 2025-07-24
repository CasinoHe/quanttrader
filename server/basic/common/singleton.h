#pragma once

#include <memory>

namespace quanttrader {

template <typename T>
class Singleton {
public:
    template<typename... Args>
    static std::shared_ptr<T> instance(Args&&... args) {
        auto& inst = get_instance_ref();
        if (!inst) {
            inst.reset(new T(std::forward<Args>(args)...), [](T* ptr) { delete ptr; });
        }
        return inst;
    }

    static void destroy_instance() {
        auto& inst = get_instance_ref();
        inst.reset();
    }

protected:
    Singleton() = default;
    ~Singleton() = default;

private:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static std::shared_ptr<T>& get_instance_ref() {
        static std::shared_ptr<T> instance;
        return instance;
    }
};
}