#pragma once
#include <algorithm>
#include <iostream>
#include <random>

template<typename T>
class Popularity {
    T value;
public:
    
    Popularity() {
        value = static_cast<T>(65 + (std::rand() % 36));
    }

    void increase(T amt) {
        value = std::min(value + amt, static_cast<T>(100));
    }

    void decrease(T amt) {
        value = std::max(value - amt, static_cast<T>(0));
    }
    T get() const { return value; }
    
    friend std::ostream& operator<<(std::ostream& os, const Popularity& p) {
        return os << p.value << "%";
    }
};
