#include <iostream>
#include <string>
#include "eval/arithmetic_eval.hpp"

int main() {
    std::cout << "=== Задание 1. Стек (Вариант 1) ===" << std::endl;
    std::cout << "Введите арифметическое выражение (или 'exit' для выхода):" << std::endl;

    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line) || line == "exit") {
            break;
        }
        if (line.empty()) continue;

        try {
            long long result = eval::evaluate_expression(line);
            std::cout << "Результат: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}