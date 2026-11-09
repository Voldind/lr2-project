#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "cache/lru_cache.hpp"

int main() {
    std::cout << "=== Задание 7. LRU кэш (Вариант 1) ===\n\n";

    // 1. Проверка
    std::cout << "--- Тест 1 (cap = 2, Q = 2) ---\n";
    {
        cache::LRUCache cache1(2);
        cache1.set(1, 2);
        std::cout << "GET 1 -> " << cache1.get(1) << " (Ожидается: 2)\n\n";
    }

    std::cout << "--- Тест 2 (cap = 2, Q = 8) ---\n";
    {
        cache::LRUCache cache2(2);
        cache2.set(1, 2);
        cache2.set(2, 3);
        cache2.set(1, 5);
        cache2.set(4, 5);
        cache2.set(6, 7);
        std::cout << "GET 4 -> " << cache2.get(4) << " (Ожидается: 5)\n";
        cache2.set(1, 2);
        std::cout << "GET 3 -> " << cache2.get(3) << " (Ожидается: -1)\n\n";
    }

    // Режим ввода
    std::cout << "--- Интерактивный режим ---\n";
    std::cout << "Введите емкость кэша (cap):\n> ";
    size_t cap;
    if (!(std::cin >> cap)) return 0;

    cache::LRUCache user_cache(cap);
    std::cout << "Кэш создан. Доступные команды:\n";
    std::cout << "  SET <key> <val>\n";
    std::cout << "  GET <key>\n";
    std::cout << "  exit\n\n";

    std::string cmd;
    while (std::cout << "> " && std::cin >> cmd && cmd != "exit") {
        if (cmd == "SET") {
            int k, v;
            std::cin >> k >> v;
            user_cache.set(k, v);
            std::cout << "OK\n";
        } else if (cmd == "GET") {
            int k;
            std::cin >> k;
            std::cout << user_cache.get(k) << "\n";
        } else {
            std::cout << "Неизвестная команда\n";
        }
    }

    return 0;
}