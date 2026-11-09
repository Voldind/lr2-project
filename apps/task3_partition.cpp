#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "set/partition.hpp"
#include "set/set.hpp"
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
    structures::Set<int> input_set;
    int val;
    while (ss >> val) {
        input_set.add(val);  // SETADD
    }

    std::cout << "Введите целевую сумму подмножества:\n> ";
    int target_sum;
    if (!(std::cin >> target_sum)) {
        std::cerr << "Ошибка: некорректное число\n";
        return 1;
    }

    structures::DynamicArray<structures::Set<int>> result_subsets;

    if (algos::PartitionSolver::partition(input_set, target_sum,
                                          result_subsets)) {
        std::cout << "\nУспешно разбито на " << result_subsets.size()
                  << " подмножеств(а):\n";
        for (size_t i = 0; i < result_subsets.size(); ++i) {
            std::cout << "Подмножество " << i + 1 << ": { ";
            std::vector<int> elems = result_subsets[i].to_vector();
            for (size_t j = 0; j < elems.size(); ++j) {
                std::cout << elems[j] << (j + 1 < elems.size() ? ", " : " ");
            }
            std::cout << "}\n";
        }
    } else {
        std::cout << "\nОшибка: множество не может быть разбито на "
                     "подмножества с суммой "
                  << target_sum << "\n";
    }

    return 0;
}