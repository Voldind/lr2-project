#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

namespace structures {

template <typename T>
class DynamicArray {
private:
    T* data;
    size_t m_size;
    size_t m_capacity;

    void resize(size_t new_cap) {
        if (new_cap == 0) new_cap = 4;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        m_capacity = new_cap;
    }

public:
    DynamicArray(size_t initial_capacity = 4) {
        if (initial_capacity == 0) initial_capacity = 4;
        m_size = 0;
        m_capacity = initial_capacity;
        data = new T[m_capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray(const DynamicArray& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            data = new T[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }

    // Добавление в конец
    void push_back(const T& value) {
        if (m_size == m_capacity) {
            resize(m_capacity * 2);
        }
        data[m_size] = value;
        m_size++;
    }

    // Вставка по индексу
    void insert(size_t index, const T& value) {
        if (index > m_size) {
            throw std::runtime_error("DynamicArray: index out of bounds");
        }
        if (m_size == m_capacity) {
            resize(m_capacity * 2);
        }
        for (size_t i = m_size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        m_size++;
    }

    // Удаление по индексу
    void remove_at(size_t index) {
        if (index >= m_size) {
            throw std::runtime_error("DynamicArray: index out of bounds");
        }
        for (size_t i = index; i < m_size - 1; ++i) {
            data[i] = data[i + 1];
        }
        m_size--;
    }

    // Замена по индексу
    void set(size_t index, const T& value) {
        if (index >= m_size) {
            throw std::runtime_error("DynamicArray: index out of bounds");
        }
        data[index] = value;
    }

    // Доступ по индексу
    T& operator[](size_t index) { return data[index]; }
    const T& operator[](size_t index) const { return data[index]; }

    T& at(size_t index) {
        if (index >= m_size) {
            throw std::runtime_error("DynamicArray: index out of bounds");
        }
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= m_size) {
            throw std::runtime_error("DynamicArray: index out of bounds");
        }
        return data[index];
    }

    void clear() {
        m_size = 0;
    }

    std::vector<T> to_vector() const {
        std::vector<T> res;
        for (size_t i = 0; i < m_size; ++i) {
            res.push_back(data[i]);
        }
        return res;
    }
};

} // namespace structures