#pragma once
#include <vector>
#include "../structures/bst.hpp"
#include "../structures/queue.hpp"
#include "../structures/array.hpp"

namespace algos {

template <typename T>
class TreeRightView {
public:
    // Функция нахождения правого края дерева (Right View)
    static structures::DynamicArray<T> get_right_view(const structures::BinarySearchTree<T>& tree) {
        structures::DynamicArray<T> result;
        if (tree.empty()) {
            return result;
        }

        // Используем нашу кастомную очередь из ЛР1
        structures::Queue<typename structures::BinarySearchTree<T>::Node*> q;
        q.push(tree.get_root());

        while (!q.empty()) {
            size_t level_size = q.size();

            for (size_t i = 0; i < level_size; ++i) {
                auto curr = q.pop();

                // Если это последний узел на текущем уровне — он виден справа
                if (i == level_size - 1) {
                    result.push_back(curr->data);
                }

                // Добавляем детей в очередь: сначала левого, затем правого
                if (curr->left != nullptr) {
                    q.push(curr->left);
                }
                if (curr->right != nullptr) {
                    q.push(curr->right);
                }
            }
        }

        return result;
    }
};

} // namespace algos