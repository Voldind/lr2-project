#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../structures/doubly_list.hpp"
#include "../structures/array.hpp"
#include "../structures/forward_list.hpp"

namespace cache {

class LRUCache {
public:
    struct CacheItem {
        int key;
        int value;
        CacheItem(int k = 0, int v = 0) : key(k), value(v) {}
    };

    using ListNode = typename structures::DoublyList<CacheItem>::Node;

private:
    struct HashEntry {
        int key;
        ListNode* node;
        HashEntry(int k = 0, ListNode* n = nullptr) : key(k), node(n) {}
    };

    // Хеш-таблица цепочками для маппинга key -> ListNode*
    structures::DynamicArray<structures::ForwardList<HashEntry>> m_map;
    size_t m_map_capacity;

    // Двусвязный список для поддержания порядка LRU (голова = свежий, хвост = старый)
    structures::DoublyList<CacheItem> m_list;
    size_t m_capacity;

    size_t hash(int key) const {
        size_t h = static_cast<size_t>(key) * 2654435761u;
        return h % m_map_capacity;
    }

    ListNode* find_in_map(int key) const {
        size_t idx = hash(key);
        const auto& bucket = m_map[idx];
        auto entries = bucket.to_vector();
        for (const auto& entry : entries) {
            if (entry.key == key) {
                return entry.node;
            }
        }
        return nullptr;
    }

    void put_in_map(int key, ListNode* node) {
        size_t idx = hash(key);
        // Если ключ уже есть в бакете, обновляем указатель
        auto entries = m_map[idx].to_vector();
        for (const auto& entry : entries) {
            if (entry.key == key) {
                // Удаляем старую запись по значению ключа
                m_map[idx].clear();
                for (const auto& e : entries) {
                    if (e.key == key) {
                        m_map[idx].push_back(HashEntry(key, node));
                    } else {
                        m_map[idx].push_back(e);
                    }
                }
                return;
            }
        }
        m_map[idx].push_back(HashEntry(key, node));
    }

    void remove_from_map(int key) {
        size_t idx = hash(key);
        auto entries = m_map[idx].to_vector();
        m_map[idx].clear();
        for (const auto& e : entries) {
            if (e.key != key) {
                m_map[idx].push_back(e);
            }
        }
    }

    // Перемещение узла в голову списка
    void move_to_head(ListNode* node) {
        if (node == m_list.get_head()) {
            return;
        }

        CacheItem item = node->data;
        m_list.erase_node(node);
        m_list.push_front(item);

        put_in_map(item.key, m_list.get_head());
    }

public:
    explicit LRUCache(size_t cap)
        : m_map_capacity(cap > 0 ? cap * 2 + 1 : 7),
          m_map(cap > 0 ? cap * 2 + 1 : 7),
          m_capacity(cap) {
        for (size_t i = 0; i < m_map_capacity; ++i) {
            m_map.push_back(structures::ForwardList<HashEntry>());
        }
    }

    // GET x: возвращает значение ключа или -1
    int get(int key) {
        if (m_capacity == 0) return -1;

        ListNode* node = find_in_map(key);
        if (node == nullptr) {
            return -1;
        }

        // Ключ найден — делаем его самым свежим
        int result_val = node->data.value;
        move_to_head(node);
        return result_val;
    }

    // SET x y: добавляет или обновляет пару ключ-значение
    void set(int key, int value) {
        if (m_capacity == 0) return;

        ListNode* node = find_in_map(key);
        if (node != nullptr) {
            // Ключ уже существует: обновляем значение
            node->data.value = value;
            move_to_head(node);
            return;
        }

        // Ключа нет. Удаляем самый старый элемент из хвоста
        if (m_list.size() == m_capacity) {
            ListNode* lru_node = m_list.get_tail();
            if (lru_node != nullptr) {
                remove_from_map(lru_node->data.key);
                m_list.pop_back();
            }
        }

        // Вставляем новый элемент в голову списка
        m_list.push_front(CacheItem(key, value));
        put_in_map(key, m_list.get_head());
    }
};

} // namespace cache