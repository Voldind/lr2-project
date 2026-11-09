#include <iostream>
#include <sstream>
#include <string>
#include "array_algos/powerset.hpp"
#include "structures/array.hpp"

int main() {
    std::cout << "=== Задание 4. Различные подмассивы (Вариант 1) ===\n";
    std::cout << "Введите элементы массива через пробел (например: x y z):\n> ";

    std::string line;
    if (!std::getline(std::cin, line) || line.empty()) {
        std::cerr << "Ошибка: пустой ввод\n";
        return 1;
    }

    std::stringstream ss(line);
    structures::DynamicArray<std::string> arr;
    std::string token;
    while (ss >> token) {
        arr.push_back(token);
    }

    if (arr.size() > 20) {
        std::cout << "Предупреждение: для N > 20 размер булеана превысит миллион элементов!\n";
    }

    // Генерация подмножеств
    structures::DynamicArray<structures::DynamicArray<std::string>> all_subsets = 
        algos::SubarrayGenerator::generate_subsets(arr);

    // Вывод
    std::cout << "\nВсего подмножеств: " << all_subsets.size() << "\nВывод: [";
    for (size_t i = 0; i < all_subsets.size(); ++i) {
        std::cout << "{";
        for (size_t j = 0; j < all_subsets[i].size(); ++j) {
            std::cout << all_subsets[i][j];
            if (j + 1 < all_subsets[i].size()) {
                std::cout << ", ";
            }
        }
        std::cout << "}";
        if (i + 1 < all_subsets.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";

    return 0;
}