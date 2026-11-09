#pragma once
#include <iostream>
#include <vector>

#include "../structures/array.hpp"
#include "set.hpp"

namespace algos {

class PartitionSolver {
   private:
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

    static bool backtrack(
        size_t idx, const structures::DynamicArray<int>& nums,
        structures::DynamicArray<structures::Set<int>>& subsets,
        structures::DynamicArray<int>& subset_sums, int target_sum) {
        if (idx == nums.size()) {
            return true;
        }

        int current_val = nums[idx];

        for (size_t i = 0; i < subsets.size(); ++i) {
            if (subset_sums[i] + current_val <= target_sum) {
                subsets[i].add(current_val);
                subset_sums[i] += current_val;
                if (backtrack(idx + 1, nums, subsets, subset_sums, target_sum))
                    return true;
                subsets[i].remove(current_val);
                subset_sums[i] -= current_val;
            }
            if (subset_sums[i] == 0) break;
        }

        return false;
    }

   public:
    static bool partition(
        const structures::Set<int>& input_set, int target_sum,
        structures::DynamicArray<structures::Set<int>>& result) {
        if (input_set.empty() || target_sum <= 0) {
            return false;
        }

        std::vector<int> raw_elements = input_set.to_vector();
        structures::DynamicArray<int> nums;
        long long total_sum = 0;

        for (int val : raw_elements) {
            if (val <= 0 || val > target_sum) {
                return false;
            }
            nums.push_back(val);
            total_sum += val;
        }

        if (total_sum % target_sum != 0) {
            return false;
        }

        size_t k = total_sum / target_sum;

        sort_descending(nums);

        structures::DynamicArray<structures::Set<int>> subsets;
        structures::DynamicArray<int> subset_sums;

        for (size_t i = 0; i < k; ++i) {
            subsets.push_back(structures::Set<int>());
            subset_sums.push_back(0);
        }

        if (backtrack(0, nums, subsets, subset_sums, target_sum)) {
            result = subsets;
            return true;
        }

        return false;
    }
};

}  // namespace algos