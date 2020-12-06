//
// Created by dotty on 12/6/20.
//

#ifndef TELE_SINGLETON_HH
#define TELE_SINGLETON_HH

template <typename T>
class Singleton {
public:
    static T& instance();

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() {}
};

template<typename T>
T& Singleton<T>::instance() {
    static T instance{};
    return instance;
}
#endif //TELE_SINGLETON_HH
