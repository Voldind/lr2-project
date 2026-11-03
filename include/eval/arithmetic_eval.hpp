#pragma once
#include <string>
#include <cctype>
#include <stdexcept>
#include "../structures/stack.hpp"

namespace eval {

// Приоритет операций
inline int get_precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*') return 2;
    return 0; // для скобок '('
}

// Применение бинарной операции к двум числам
inline long long apply_op(long long a, long long b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        default: throw std::runtime_error("Unknown operator");
    }
}

// Вспомогательная функция: выполнение одной операции с вершины стека
inline void process_top_operation(structures::Stack<long long>& values, structures::Stack<char>& ops) {
    if (values.size() < 2) {
        throw std::runtime_error("Expression syntax error: not enough operands");
    }
    long long b = values.pop(); // правый операнд
    long long a = values.pop(); // левый операнд
    char op = ops.pop();
    values.push(apply_op(a, b, op));
}

// Главная функция вычисления выражения
inline long long evaluate_expression(const std::string& expr) {
    structures::Stack<long long> values;
    structures::Stack<char> ops;

    bool may_be_unary = true; // флаг: ожидается ли число/унарный знак

    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];

        // 1. Пропускаем пробелы
        if (std::isspace(static_cast<unsigned char>(ch))) {
            continue;
        }

        // 2. Обработка открывающей скобки
        if (ch == '(') {
            ops.push(ch);
            may_be_unary = true;
        }
        // 3. Обработка закрывающей скобки
        else if (ch == ')') {
            bool found_open_paren = false;
            while (!ops.empty()) {
                if (ops.top() == '(') {
                    found_open_paren = true;
                    ops.pop(); // убираем '('
                    break;
                }
                process_top_operation(values, ops);
            }
            if (!found_open_paren) {
                throw std::runtime_error("Mismatched parentheses: missing '('");
            }
            may_be_unary = false;
        }
        // 4. Считывание числа (включая унарный плюс/минус перед числом)
        else if (std::isdigit(static_cast<unsigned char>(ch)) || (may_be_unary && (ch == '+' || ch == '-'))) {
            long long sign = 1;
            if (ch == '-') {
                sign = -1;
                i++;
            } else if (ch == '+') {
                i++;
            }

            // Пропускаем пробелы между унарным знаком и числом, если они есть
            while (i < expr.length() && std::isspace(static_cast<unsigned char>(expr[i]))) {
                i++;
            }

            if (i >= expr.length() || !std::isdigit(static_cast<unsigned char>(expr[i]))) {
                throw std::runtime_error("Syntax error: expected number after unary operator");
            }

            long long num = 0;
            while (i < expr.length() && std::isdigit(static_cast<unsigned char>(expr[i]))) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            i--; // компенсируем инкремент внешнего цикла for

            values.push(num * sign);
            may_be_unary = false;
        }
        // 5. Бинарные операторы: +, -, *
        else if (ch == '+' || ch == '-' || ch == '*') {
            if (may_be_unary) {
                throw std::runtime_error("Syntax error: unexpected binary operator");
            }

            while (!ops.empty() && get_precedence(ops.top()) >= get_precedence(ch)) {
                process_top_operation(values, ops);
            }
            ops.push(ch);
            may_be_unary = true;
        }
        else {
            throw std::runtime_error(std::string("Invalid character encountered: ") + ch);
        }
    }

    // Довычисляем все оставшиеся в стеке операции
    while (!ops.empty()) {
        if (ops.top() == '(') {
            throw std::runtime_error("Mismatched parentheses: unclosed '('");
        }
        process_top_operation(values, ops);
    }

    if (values.size() != 1) {
        throw std::runtime_error("Invalid expression: unbalanced operands");
    }

    return values.pop();
}

} // namespace eval