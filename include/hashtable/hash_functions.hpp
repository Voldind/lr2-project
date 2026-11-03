#pragma once
#include <string>
#include <cstddef>
#include <cstdint>

namespace hashtable {

// Метод свёртки (Folding Method) для строк
inline size_t folding_hash(const std::string& key, size_t table_size) {
    if (table_size == 0) return 0;

    size_t sum = 0;
    size_t i = 0;

    // Сворачиваем по 2 символа (16 бит на блок)
    while (i + 1 < key.length()) {
        uint16_t block = (static_cast<uint8_t>(key[i]) << 8) | static_cast<uint8_t>(key[i + 1]);
        sum += block;
        i += 2;
    }

    // Добавляем оставшийся нечётный символ, если есть
    if (i < key.length()) {
        sum += static_cast<uint8_t>(key[i]);
    }

    return sum % table_size;
}

// Вторичная хеш-функция для шага двойного хеширования (h2 > 0)
inline size_t secondary_hash(const std::string& key, size_t table_size) {
    if (table_size <= 1) return 1;

    size_t h = 0;
    for (char c : key) {
        h = h * 31 + static_cast<uint8_t>(c);
    }
    // Шаг должен быть в диапазоне [1, table_size - 1] и не равен 0
    return 1 + (h % (table_size - 1));
}

} // namespace hashtable