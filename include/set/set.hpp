#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "../structures/array.hpp"
#include "../structures/forward_list.hpp"

namespace structures {

class Set {
private:
    DynamicArray<ForwardList<std::string>> m_buckets;
    size_t m_size;
    size_t m_capacity;

    // Хеш-функция djb2 для строк
    size_t hash(const std::string& key) const {
        size_t h = 5381;
        for (char c : key) {
            h = ((h << 5) + h) + static_cast<unsigned char>(c);
        }
        return h % m_capacity;
    }

    void rehash() {
        size_t old_cap = m_capacity;
        m_capacity *= 2;
        DynamicArray<ForwardList<std::string>> new_buckets(m_capacity);
        for (size_t i = 0; i < m_capacity; ++i) {
            new_buckets.push_back(ForwardList<std::string>());
        }

        for (size_t i = 0; i < old_cap; ++i) {
            std::vector<std::string> elements = m_buckets[i].to_vector();
            for (const auto& elem : elements) {
                size_t new_idx = 5381;
                for (char c : elem) {
                    new_idx = ((new_idx << 5) + new_idx) + static_cast<unsigned char>(c);
                }
                new_idx %= m_capacity;
                new_buckets[new_idx].push_back(elem);
            }
        }
        m_buckets = new_buckets;
    }

public:
    explicit Set(size_t initial_capacity = 16)
        : m_buckets(initial_capacity), m_size(0), m_capacity(initial_capacity) {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_buckets.push_back(ForwardList<std::string>());
        }
    }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    // SETADD: добавление элемента в множество
    bool add(const std::string& value) {
        if (contains(value)) {
            return false;
        }
        if (m_size + 1 > m_capacity * 0.75) {
            rehash();
        }
        size_t idx = hash(value);
        m_buckets[idx].push_back(value);
        m_size++;
        return true;
    }

    // SETDEL: удаление элемента из множества
    bool remove(const std::string& value) {
        size_t idx = hash(value);
        if (m_buckets[idx].find(value) != -1) {
            m_buckets[idx].remove_by_value(value);
            m_size--;
            return true;
        }
        return false;
    }

    // SET_AT: проверка наличия элемента в множестве
    bool contains(const std::string& value) const {
        size_t idx = hash(value);
        return m_buckets[idx].find(value) != -1;
    }

    std::vector<std::string> to_vector() const {
        std::vector<std::string> all_elements;
        all_elements.reserve(m_size);
        for (size_t i = 0; i < m_capacity; ++i) {
            std::vector<std::string> b_elems = m_buckets[i].to_vector();
            for (const auto& e : b_elems) {
                all_elements.push_back(e);
            }
        }
        return all_elements;
    }

    void clear() {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_buckets[i].clear();
        }
        m_size = 0;
    }
};

} // namespace structures