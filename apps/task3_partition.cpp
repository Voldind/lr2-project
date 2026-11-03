#include <iostream>
#include <sstream>
#include <string>
#include "set/partition.hpp"
#include "structures/array.hpp"

int main() {
    std::cout << "=== Задание 3. Разбиение множества (Вариант 1) ===\n";
    std::cout << "Введите элементы множества через пробел:\n> ";

    std::string line;
    if (!std::getline(std::cin, line) || line.empty()) {
        std::cerr << "Ошибка: пустой ввод\n";
        return 1;
    }

    std::stringstream ss(line);
    structures::DynamicArray<int> nums;
    int val;
    while (ss >> val) {
        nums.push_back(val);
    }

    std::cout << "Введите целевую сумму подмножества:\n> ";
    int target_sum;
    if (!(std::cin >> target_sum)) {
        std::cerr << "Ошибка: некорректное число\n";
        return 1;
    }

    structures::DynamicArray<structures::DynamicArray<int>> result;

    if (algos::PartitionSolver::partition(nums, target_sum, result)) {
        std::cout << "\nУспешно разбито на " << result.size() << " подмножеств(а):\n";
        for (size_t i = 0; i < result.size(); ++i) {
            std::cout << "Подмножество " << i + 1 << ": { ";
            for (size_t j = 0; j < result[i].size(); ++j) {
                std::cout << result[i][j] << (j + 1 < result[i].size() ? ", " : " ");
            }
            std::cout << "}\n";
        }
    } else {
        std::cout << "\nОшибка: множество не может быть разбито на подмножества с суммой " 
                  << target_sum << "\n";
    }

    return 0;
}