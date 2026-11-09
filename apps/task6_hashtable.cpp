#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "hashtable/open_addressing_table.hpp"

// Метод свертки
void demo_folding_example() {
    std::cout << "--- Метод свертки (Folding) ---\n";
    std::string num_key = "523456795";
    // Разбиение на 3 блока по 3 цифры
    int p1 = 523, p2 = 456, p3 = 795;
    int sum = p1 + p2 + p3;
    std::cout << "Пример ключа: " << num_key << "\n";
    std::cout << "Свертка: " << p1 << " + " << p2 << " + " << p3 << " = " << sum << "\n\n";
}

int main() {
    std::cout << "=== Задание 6. Хеш-таблица (Вариант 1) ===\n\n";

    // Поясняющий пример метода свертки
    demo_folding_example();

    // Сравнение методов разрешения коллизий
    std::cout << "--- Сравнение методов (число проб при вставке) ---\n";

    std::vector<std::string> keys = {
        "apple", "application", "apply", "banana", "band",
        "cat", "caterpillar", "dog", "door", "forest"
    };

    size_t initial_cap = 13;
    hashtable::OpenAddressingTable<int> linear_table(initial_cap, hashtable::CollisionResolution::LINEAR);
    hashtable::OpenAddressingTable<int> double_table(initial_cap, hashtable::CollisionResolution::DOUBLE_HASHING);

    size_t total_linear = 0;
    size_t total_double = 0;

    std::cout << std::left << std::setw(16) << "Ключ" 
              << std::setw(18) << "Линейное (проб)" 
              << "Двойное (проб)\n";
    std::cout << std::string(48, '-') << "\n";

    for (size_t i = 0; i < keys.size(); ++i) {
        size_t p_lin = linear_table.insert(keys[i], static_cast<int>(i + 1));
        size_t p_dbl = double_table.insert(keys[i], static_cast<int>(i + 1));

        total_linear += p_lin;
        total_double += p_dbl;

        std::cout << std::left << std::setw(16) << keys[i] 
                  << std::setw(18) << p_lin 
                  << p_dbl << "\n";
    }

    std::cout << std::string(48, '-') << "\n";
    std::cout << std::left << std::setw(16) << "ИТОГО шагов:" 
              << std::setw(18) << total_linear 
              << total_double << "\n\n";

    // Проверка основных операций
    std::cout << "--- Проверка операций (Линейное пробирование) ---\n";
    hashtable::OpenAddressingTable<std::string> table(11, hashtable::CollisionResolution::LINEAR);

    table.insert("user1", "Alice");
    table.insert("user2", "Bob");
    std::cout << "Добавлены: user1 -> Alice, user2 -> Bob\n";

    std::string val;
    if (table.find("user1", val)) {
        std::cout << "Поиск 'user1': найдено (" << val << ")\n";
    }

    table.remove("user1");
    std::cout << "Удаление 'user1': выполнено\n";

    if (!table.find("user1", val)) {
        std::cout << "Поиск 'user1': не найден (OK)\n";
    }

    std::cout << "\nВывод: Двойное хеширование минимизирует кластеризацию и требует меньше шагов.\n";

    return 0;
}