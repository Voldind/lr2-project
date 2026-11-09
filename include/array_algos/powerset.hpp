#pragma once
#include <string>
#include "../structures/array.hpp"

namespace algos {

class SubarrayGenerator {
public:
    // Генерация всех подмножеств для массива произвольного типа
    template <typename T>
    static structures::DynamicArray<structures::DynamicArray<T>> generate_subsets(const structures::DynamicArray<T>& arr) {
        structures::DynamicArray<structures::DynamicArray<T>> result;
        size_t n = arr.size();
        
        unsigned long long total_subsets = 1ULL << n;

        for (unsigned long long mask = 0; mask < total_subsets; ++mask) {
            structures::DynamicArray<T> current_subset;
            for (size_t j = 0; j < n; ++j) {
                // Если j-й бит маски установлен в 1
                if ((mask >> j) & 1ULL) {
                    current_subset.push_back(arr[j]);
                }
            }
            result.push_back(current_subset);
        }

        return result;
    }
};

} // namespace algos