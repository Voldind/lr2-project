#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

namespace structures {

template <typename T>
class DoublyList {
public:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& val, Node* n = nullptr, Node* p = nullptr)
            : data(val), next(n), prev(p) {}
    };

private:
    Node* head;
    Node* tail;
    size_t m_size;

    Node* getNode(const T& target) const {
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data == target) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

public:
    DoublyList() : head(nullptr), tail(nullptr), m_size(0) {}

    ~DoublyList() {
        clear();
    }

    DoublyList(const DoublyList& other) : head(nullptr), tail(nullptr), m_size(0) {
        Node* curr = other.head;
        while (curr != nullptr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    DoublyList& operator=(const DoublyList& other) {
        if (this != &other) {
            clear();
            Node* curr = other.head;
            while (curr != nullptr) {
                push_back(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    size_t size() const { return m_size; }
    bool empty() const { return head == nullptr; }

    Node* get_head() const { return head; }
    Node* get_tail() const { return tail; }

    void push_front(const T& value) {
        Node* newNode = new Node(value, head, nullptr);
        if (empty()) {
            head = tail = newNode;
        } else {
            head->prev = newNode;
            head = newNode;
        }
        m_size++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value, nullptr, tail);
        if (empty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        m_size++;
    }

    void pop_front() {
        if (empty()) {
            throw std::runtime_error("DoublyList is empty");
        }
        Node* toDelete = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }
        delete toDelete;
        m_size--;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("DoublyList is empty");
        }
        Node* toDelete = tail;
        tail = tail->prev;
        if (tail == nullptr) {
            head = nullptr;
        } else {
            tail->next = nullptr;
        }
        delete toDelete;
        m_size--;
    }

    // Вставка после target
    void insert_after(const T& target, const T& value) {
        Node* targetNode = getNode(target);
        if (targetNode == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        if (targetNode == tail) {
            push_back(value);
            return;
        }
        Node* newNode = new Node(value, targetNode->next, targetNode);
        targetNode->next->prev = newNode;
        targetNode->next = newNode;
        m_size++;
    }

    // Вставка до target
    void insert_before(const T& target, const T& value) {
        Node* targetNode = getNode(target);
        if (targetNode == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        if (targetNode == head) {
            push_front(value);
            return;
        }
        Node* newNode = new Node(value, targetNode, targetNode->prev);
        targetNode->prev->next = newNode;
        targetNode->prev = newNode;
        m_size++;
    }

    // Удаление после target
    void remove_after(const T& target) {
        Node* targetNode = getNode(target);
        if (targetNode == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        if (targetNode == tail || targetNode->next == nullptr) {
            throw std::runtime_error("No element exists after target");
        }
        if (targetNode->next == tail) {
            pop_back();
            return;
        }
        Node* toDelete = targetNode->next;
        targetNode->next = toDelete->next;
        toDelete->next->prev = targetNode;
        delete toDelete;
        m_size--;
    }

    // Удаление до target
    void remove_before(const T& target) {
        Node* targetNode = getNode(target);
        if (targetNode == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        if (targetNode == head || targetNode->prev == nullptr) {
            throw std::runtime_error("No element exists before target");
        }
        if (targetNode->prev == head) {
            pop_front();
            return;
        }
        Node* toDelete = targetNode->prev;
        targetNode->prev = toDelete->prev;
        toDelete->prev->next = targetNode;
        delete toDelete;
        m_size--;
    }

    // Удаление элемента по значению
    void remove_by_value(const T& target) {
        Node* targetNode = getNode(target);
        if (targetNode == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        if (targetNode == head) {
            pop_front();
            return;
        }
        if (targetNode == tail) {
            pop_back();
            return;
        }
        targetNode->prev->next = targetNode->next;
        targetNode->next->prev = targetNode->prev;
        delete targetNode;
        m_size--;
    }

    // Удаление конкретного указателя на узел за O(1) (для LRU кэша)
    void erase_node(Node* node) {
        if (node == nullptr) return;
        if (node == head) {
            pop_front();
            return;
        }
        if (node == tail) {
            pop_back();
            return;
        }
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
        m_size--;
    }

    T& front() {
        if (empty()) throw std::runtime_error("DoublyList is empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("DoublyList is empty");
        return head->data;
    }

    T& back() {
        if (empty()) throw std::runtime_error("DoublyList is empty");
        return tail->data;
    }

    const T& back() const {
        if (empty()) throw std::runtime_error("DoublyList is empty");
        return tail->data;
    }

    T& at(size_t index) {
        if (index >= m_size) throw std::runtime_error("Index out of bounds");
        if (index < m_size / 2) {
            Node* curr = head;
            for (size_t i = 0; i < index; ++i) curr = curr->next;
            return curr->data;
        } else {
            Node* curr = tail;
            for (size_t i = m_size - 1; i > index; --i) curr = curr->prev;
            return curr->data;
        }
    }

    int find(const T& target) const {
        Node* curr = head;
        int idx = 0;
        while (curr != nullptr) {
            if (curr->data == target) return idx;
            curr = curr->next;
            idx++;
        }
        return -1;
    }

    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        m_size = 0;
    }

    std::vector<T> to_vector() const {
        std::vector<T> res;
        Node* curr = head;
        while (curr != nullptr) {
            res.push_back(curr->data);
            curr = curr->next;
        }
        return res;
    }
};

} // namespace structures