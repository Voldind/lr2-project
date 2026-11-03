#pragma once
#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "../structures/array.hpp"
#include "hash_functions.hpp"

namespace hashtable {

enum class CollisionResolution {
    LINEAR,
    DOUBLE_HASHING
};

enum class EntryState {
    EMPTY,
    OCCUPIED,
    DELETED
};

template <typename V>
class OpenAddressingTable {
public:
    struct Entry {
        std::string key;
        V value;
        EntryState state;

        Entry() : key(""), value(V()), state(EntryState::EMPTY) {}
        Entry(const std::string& k, const V& v) : key(k), value(v), state(EntryState::OCCUPIED) {}
    };

private:
    structures::DynamicArray<Entry> m_table;
    size_t m_size;
    size_t m_capacity;
    CollisionResolution m_strategy;

    // Вычисление i-го индекса проб
    size_t get_probe_index(const std::string& key, size_t i) const {
        size_t h1 = folding_hash(key, m_capacity);

        if (m_strategy == CollisionResolution::LINEAR) {
            return (h1 + i) % m_capacity;
        } else {
            size_t h2 = secondary_hash(key, m_capacity);
            return (h1 + i * h2) % m_capacity;
        }
    }

    void rehash() {
        size_t old_cap = m_capacity;
        // Берём нечётное число для лучшей взаимной простоты при двойном хешировании
        m_capacity = m_capacity * 2 + 1;

        structures::DynamicArray<Entry> old_table = m_table;
        m_table = structures::DynamicArray<Entry>(m_capacity);
        for (size_t i = 0; i < m_capacity; ++i) {
            m_table.push_back(Entry());
        }
        m_size = 0;

        for (size_t i = 0; i < old_cap; ++i) {
            if (old_table[i].state == EntryState::OCCUPIED) {
                insert(old_table[i].key, old_table[i].value);
            }
        }
    }

public:
    explicit OpenAddressingTable(size_t initial_capacity = 17, 
                                 CollisionResolution strategy = CollisionResolution::LINEAR)
        : m_table(initial_capacity), m_size(0), m_capacity(initial_capacity), m_strategy(strategy) {
        for (size_t i = 0; i < m_capacity; ++i) {
            m_table.push_back(Entry());
        }
    }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }

    // Вставка ключа и значения. Возвращает количество сделанных проб (для анализа коллизий)
    size_t insert(const std::string& key, const V& value) {
        // Поддержание коэффициента загрузки <= 0.7
        if (m_size + 1 > m_capacity * 0.7) {
            rehash();
        }

        int first_deleted_idx = -1;
        size_t probes = 0;

        for (size_t i = 0; i < m_capacity; ++i) {
            probes++;
            size_t idx = get_probe_index(key, i);

            if (m_table[idx].state == EntryState::OCCUPIED) {
                if (m_table[idx].key == key) {
                    m_table[idx].value = value; // Обновляем значение существующего ключа
                    return probes;
                }
            } else if (m_table[idx].state == EntryState::DELETED) {
                if (first_deleted_idx == -1) {
                    first_deleted_idx = static_cast<int>(idx);
                }
            } else if (m_table[idx].state == EntryState::EMPTY) {
                // Если ранее встретили DELETED-ячейку — занимаем её
                size_t insert_idx = (first_deleted_idx != -1) ? static_cast<size_t>(first_deleted_idx) : idx;
                m_table[insert_idx] = Entry(key, value);
                m_size++;
                return probes;
            }
        }

        // Если все ячейки в цикле были заняты или удалены
        if (first_deleted_idx != -1) {
            m_table[static_cast<size_t>(first_deleted_idx)] = Entry(key, value);
            m_size++;
            return probes;
        }

        throw std::runtime_error("HashTable is full, failed to insert");
    }

    // Поиск значения по ключу
    bool find(const std::string& key, V& out_val) const {
        for (size_t i = 0; i < m_capacity; ++i) {
            size_t idx = get_probe_index(key, i);

            if (m_table[idx].state == EntryState::EMPTY) {
                return false; // Дошли до пустой — элемента точно нет
            }
            if (m_table[idx].state == EntryState::OCCUPIED && m_table[idx].key == key) {
                out_val = m_table[idx].value;
                return true;
            }
        }
        return false;
    }

    // Удаление ключа
    bool remove(const std::string& key) {
        for (size_t i = 0; i < m_capacity; ++i) {
            size_t idx = get_probe_index(key, i);

            if (m_table[idx].state == EntryState::EMPTY) {
                return false;
            }
            if (m_table[idx].state == EntryState::OCCUPIED && m_table[idx].key == key) {
                m_table[idx].state = EntryState::DELETED;
                m_table[idx].key = "";
                m_size--;
                return true;
            }
        }
        return false;
    }

    void print_table() const {
        std::cout << "--- Состояние Хеш-Таблицы (Размер: " << m_size << "/" << m_capacity << ") ---\n";
        for (size_t i = 0; i < m_capacity; ++i) {
            std::cout << "[" << i << "]: ";
            if (m_table[i].state == EntryState::OCCUPIED) {
                std::cout << "{" << m_table[i].key << ": " << m_table[i].value << "}\n";
            } else if (m_table[i].state == EntryState::DELETED) {
                std::cout << "<DELETED>\n";
            } else {
                std::cout << "EMPTY\n";
            }
        }
    }
};

} // namespace hashtable