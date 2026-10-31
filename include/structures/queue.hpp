#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include "doubly_list.hpp"

namespace structures {

template <typename T>
class Queue {
private:
    DoublyList<T> list;

public:
    Queue() = default;

    bool empty() const { return list.empty(); }
    size_t size() const { return list.size(); }

    void push(const T& value) {
        list.push_back(value);
    }

    T pop() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        T frontVal = list.front();
        list.pop_front();
        return frontVal;
    }

    T& front() {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.front();
    }

    const T& front() const {
        if (empty()) {
            throw std::runtime_error("Queue is empty");
        }
        return list.front();
    }

    void clear() {
        list.clear();
    }

    std::vector<T> to_vector() const {
        return list.to_vector();
    }
};

} // namespace structures