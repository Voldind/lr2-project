#pragma once
#include <iostream>
#include <string>
#include "../structures/array.hpp"

namespace algos {

class PartitionSolver {
private:
    // Вспомогательная сортировка по убыванию
    static void sort_descending(structures::DynamicArray<int>& arr) {
        for (size_t i = 0; i < arr.size(); ++i) {
            for (size_t j = i + 1; j < arr.size(); ++j) {
                if (arr[j] > arr[i]) {
                    int tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
                }
            }
        }
    }

    // Рекурсивный перебор (backtracking)
    static bool backtrack(size_t idx, 
                          const structures::DynamicArray<int>& nums, 
                          structures::DynamicArray<structures::DynamicArray<int>>& subsets,
                          structures::DynamicArray<int>& subset_sums, 
                          int target_sum) {
        if (idx == nums.size()) {
            return true; // все числа успешно распределены
        }

        int current_val = nums[idx];

        for (size_t i = 0; i < subsets.size(); ++i) {
            // Проверка, помещается ли число в i-ю корзину
            if (subset_sums[i] + current_val <= target_sum) {
                subsets[i].push_back(current_val);
                subset_sums[i] += current_val;

                if (backtrack(idx + 1, nums, subsets, subset_sums, target_sum)) {
                    return true;
                }

                // Откат назад
                subsets[i].remove_at(subsets[i].size() - 1);
                subset_sums[i] -= current_val;
            }

            // Оптимизация: если корзина была пустой и число в неё не подошло,
            // другие пустые корзины проверять бессмысленно (они идентичны)
            if (subset_sums[i] == 0) {
                break;
            }
        }

        return false;
    }

public:
    // Основной метод разбиения
    static bool partition(structures::DynamicArray<int>& nums, 
                          int target_sum, 
                          structures::DynamicArray<structures::DynamicArray<int>>& result) {
        if (nums.empty() || target_sum <= 0) {
            return false;
        }

        long long total_sum = 0;
        for (size_t i = 0; i < nums.size(); ++i) {
            if (nums[i] <= 0) {
                return false; // По условию числа натуральные
            }
            if (nums[i] > target_sum) {
                return false; // Элемент больше целевой суммы
            }
            total_sum += nums[i];
        }

        if (total_sum % target_sum != 0) {
            return false; // Сумма не кратна целевой сумме
        }

        size_t k = total_sum / target_sum; // Количество подмножеств

        // Сортировка по убыванию для ускорения отсечений
        structures::DynamicArray<int> sorted_nums = nums;
        sort_descending(sorted_nums);

        structures::DynamicArray<structures::DynamicArray<int>> subsets;
        structures::DynamicArray<int> subset_sums;

        for (size_t i = 0; i < k; ++i) {
            subsets.push_back(structures::DynamicArray<int>());
            subset_sums.push_back(0);
        }

        if (backtrack(0, sorted_nums, subsets, subset_sums, target_sum)) {
            result = subsets;
            return true;
        }

        return false;
    }
};

} // namespace algos