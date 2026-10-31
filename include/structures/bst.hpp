#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

namespace structures {

template <typename T>
class BinarySearchTree {
public:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& val) : data(val), left(nullptr), right(nullptr) {}
    };

private:
    Node* root;
    size_t m_size;

    Node* insertNode(Node* node, const T& value) {
        if (node == nullptr) {
            m_size++;
            return new Node(value);
        }
        if (value < node->data) {
            node->left = insertNode(node->left, value);
        } else if (value > node->data) {
            node->right = insertNode(node->right, value);
        }
        return node;
    }

    bool findNode(Node* node, const T& value) const {
        if (node == nullptr) return false;
        if (node->data == value) return true;
        if (value < node->data) return findNode(node->left, value);
        return findNode(node->right, value);
    }

    void clearTree(Node* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }

    void inorderTraversal(Node* node, std::vector<T>& res) const {
        if (node != nullptr) {
            inorderTraversal(node->left, res);
            res.push_back(node->data);
            inorderTraversal(node->right, res);
        }
    }

public:
    BinarySearchTree() : root(nullptr), m_size(0) {}

    ~BinarySearchTree() {
        clear();
    }

    size_t size() const { return m_size; }
    bool empty() const { return root == nullptr; }
    Node* get_root() const { return root; }

    void insert(const T& value) {
        root = insertNode(root, value);
    }

    bool find(const T& value) const {
        return findNode(root, value);
    }

    void clear() {
        clearTree(root);
        root = nullptr;
        m_size = 0;
    }

    std::vector<T> to_vector() const {
        std::vector<T> res;
        inorderTraversal(root, res);
        return res;
    }
};

} // namespace structures