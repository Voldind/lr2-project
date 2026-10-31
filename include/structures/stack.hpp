#pragma once
#include <stdexcept>
#include "array.hpp"

namespace structures {

template <typename T>
class Stack {
private:
    DynamicArray<T> data;

public:
    Stack(size_t initial_capacity = 4) : data(initial_capacity) {}

    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }

    void push(const T& value) {
        data.push_back(value);
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        T topVal = data[data.size() - 1];
        data.remove_at(data.size() - 1);
        return topVal;
    }

    T& top() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data[data.size() - 1];
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data[data.size() - 1];
    }

    void clear() {
        data.clear();
    }
};

} // namespace structures