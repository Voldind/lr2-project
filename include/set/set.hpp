#pragma once
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "../structures/array.hpp"
#include "../structures/forward_list.hpp"

namespace structures {

template <typename T = std::string>
class Set {
   private:
    DynamicArray<ForwardList<T>> m_buckets;
    size_t m_size;
    size_t m_capacity;

    // Хеш-функция: для строк - djb2, для чисел - умножение на константу
    size_t hash(const T& key) const {
        if constexpr (std::is_same_v<T, std::string>) {
            size_t h = 5381;
            for (char c : key) {
                h = ((h << 5) + h) + static_cast<unsigned char>(c);
            }
            return h % m_capacity;
        } else {
            // Для целых чисел (методика Кнута / умножение)
            size_t h = static_cast<size_t>(key) * 2654435761u;
            return h % m_capacity;
        }
    }

    void rehash() {
        size_t old_cap = m_capacity;
        m_capacity *= 2;
        DynamicArray<ForwardList<T>> new_buckets(m_capacity);
        for (size_t i = 0; i < m_capacity; ++i) {
            new_buckets.push_back(ForwardList<T>());
        }

        for (size_t i = 0; i < old_cap; ++i) {
            std::vector<T> elements = m_buckets[i].to_vector();
            for (const auto& elem : elements) {
                size_t new_idx = 0;
                if constexpr (std::is_same_v<T, std::string>) {
                    size_t h = 5381;
                    for (char c : elem) {
                        h = ((h << 5) + h) + static_cast<unsigned char>(c);
                    }
                    new_idx = h % m_capacity;
                } else {
                    new_idx =
                        (static_cast<size_t>(elem) * 2654435761u) % m_capacity;
                }
                new_buckets[new_idx].push_back(elem);
            }
        }
        m_buckets = new_buckets;
    }

   public:
    explicit Set(size_t initial_capacity = 16)
        : m_buckets(initial_capacity), m_size(0), m_capacity(initial_capacity) {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_buckets.push_back(ForwardList<T>());
        }
    }

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    // SETADD: добавление элемента в множество (O(1))
    bool add(const T& value) {
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

    // SETDEL: удаление элемента из множества (O(1))
    bool remove(const T& value) {
        size_t idx = hash(value);
        if (m_buckets[idx].find(value) != -1) {
            m_buckets[idx].remove_by_value(value);
            m_size--;
            return true;
        }
        return false;
    }

    // SET_AT: проверка наличия элемента в множестве (O(1))
    bool contains(const T& value) const {
        size_t idx = hash(value);
        return m_buckets[idx].find(value) != -1;
    }

    std::vector<T> to_vector() const {
        std::vector<T> all_elements;
        all_elements.reserve(m_size);
        for (size_t i = 0; i < m_capacity; ++i) {
            std::vector<T> b_elems = m_buckets[i].to_vector();
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

}  // namespace structures