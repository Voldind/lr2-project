#include <iostream>
#include <string>

#include "cache/lru_cache.hpp"

int main() {
    std::cout << "=== Задание 7. LRU-кэш ===\n";
    std::cout << "Введите ёмкость кэша Cap и количество запросов Q\n";
    size_t cap = 0;
    int q = 0;

    if (!(std::cin >> cap >> q)) {
        return 0;
    }

    cache::LRUCache lru(cap);

    for (int i = 0; i < q; ++i) {
        std::string op;
        std::cin >> op;

        if (op == "SET") {
            int key, val;
            std::cin >> key >> val;
            lru.set(key, val);
        } else if (op == "GET") {
            int key;
            std::cin >> key;
            std::cout << lru.get(key) << " ";
        }
    }
    std::cout << "\n";

    return 0;
}