#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "hashtable/open_addressing_table.hpp"

void demo_folding_example() {
    std::cout << "Метод свертки (Folding)\n";
    std::string num_key = "523456795";
    int p1 = 523, p2 = 456, p3 = 795;
    std::cout << "Пример ключа: " << num_key << "\n";
    std::cout << "Свертка: " << p1 << " + " << p2 << " + " << p3 << " = "
              << (p1 + p2 + p3) << "\n\n";
}

int main() {
    std::cout << "=== Задание 6. Хеш-таблица ===\n\n";

    demo_folding_example();

    const int N = 100;
    std::vector<std::string> keys;
    keys.reserve(N);
    for (int i = 0; i < N; ++i) {
        keys.push_back("user_key_" + std::to_string(i * 17 + 3));
    }

    std::cout << "Эмпирический анализ на " << N << " элементах\n";

    hashtable::OpenAddressingTable<int> linear_table(
        101, hashtable::CollisionResolution::LINEAR);
    hashtable::OpenAddressingTable<int> double_table(
        101, hashtable::CollisionResolution::DOUBLE_HASHING);

    size_t linear_probes = 0;
    size_t double_probes = 0;

    auto start_lin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        linear_probes += linear_table.insert(keys[i], i);
    }
    for (int i = 0; i < N; ++i) {
        int val;
        linear_table.find(keys[i], val);
    }
    auto end_lin = std::chrono::high_resolution_clock::now();
    double time_lin_ms =
        std::chrono::duration<double, std::milli>(end_lin - start_lin).count();

    auto start_dbl = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        double_probes += double_table.insert(keys[i], i);
    }
    for (int i = 0; i < N; ++i) {
        int val;
        double_table.find(keys[i], val);
    }
    auto end_dbl = std::chrono::high_resolution_clock::now();
    double time_dbl_ms =
        std::chrono::duration<double, std::milli>(end_dbl - start_dbl).count();

    std::cout << "Метод\t\t\tШагов (проб)\tВремя (мс)\n";
    std::cout << "Линейное пробирование:\t" << linear_probes << "\t\t"
              << time_lin_ms << " мс\n";
    std::cout << "Двойное хеширование:\t" << double_probes << "\t\t"
              << time_dbl_ms << " мс\n";
    std::cout << std::endl;

    std::cout << "Проверка операций (find / remove)\n";
    std::string test_key = "user_key_3";
    int found_val = -1;

    if (linear_table.find(test_key, found_val)) {
        std::cout << "Поиск '" << test_key
                  << "': найдено (значение = " << found_val << ")\n";
    }

    linear_table.remove(test_key);
    std::cout << "Удаление '" << test_key << "': выполнено\n";

    if (!linear_table.find(test_key, found_val)) {
        std::cout << "Повторный поиск '" << test_key << "': не найден (OK)\n\n";
    }

    std::cout << "Вывод: Двойное хеширование минимизирует первичную "
                 "кластеризацию и сокращает число проб.\n";

    return 0;
}