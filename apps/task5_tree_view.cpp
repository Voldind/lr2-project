#include <iostream>
#include <sstream>
#include <string>
#include "structures/bst.hpp"
#include "structures/array.hpp"
#include "tree/right_view.hpp"

int main() {
    std::cout << "=== Задание 5. Правый край бинарного дерева (Вариант 1) ===\n";
    std::cout << "Введите числа для построения дерева поиска через пробел:\n";

    std::string line;
    if (!std::getline(std::cin, line) || line.empty()) {
        std::cerr << "Ошибка: пустой ввод\n";
        return 1;
    }

    std::stringstream ss(line);
    structures::BinarySearchTree<int> bst;
    int val;
    while (ss >> val) {
        bst.insert(val);
    }

    structures::DynamicArray<int> right_view = algos::TreeRightView<int>::get_right_view(bst);

    std::cout << "\nРезультат: ";
    for (size_t i = 0; i < right_view.size(); ++i) {
        std::cout << right_view[i];
        if (i + 1 < right_view.size()) {
            std::cout << ", ";
        }
    }
    std::cout << "\n";

    return 0;
}