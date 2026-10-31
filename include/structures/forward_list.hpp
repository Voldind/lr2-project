#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>

namespace structures {

template <typename T>
class ForwardList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* n = nullptr) : data(val), next(n) {}
    };

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
    ForwardList() : head(nullptr), tail(nullptr), m_size(0) {}

    ~ForwardList() {
        clear();
    }

    // Конструктор копирования
    ForwardList(const ForwardList& other) : head(nullptr), tail(nullptr), m_size(0) {
        Node* curr = other.head;
        while (curr != nullptr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    ForwardList& operator=(const ForwardList& other) {
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

    void push_front(const T& value) {
        Node* newNode = new Node(value, head);
        head = newNode;
        if (tail == nullptr) tail = newNode;
        m_size++;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value, nullptr);
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
            throw std::runtime_error("ForwardList is empty");
        }
        Node* toDelete = head;
        head = head->next;
        delete toDelete;
        m_size--;
        if (head == nullptr) tail = nullptr;
    }

    void pop_back() {
        if (empty()) {
            throw std::runtime_error("ForwardList is empty");
        }
        if (head == tail) {
            pop_front();
            return;
        }
        Node* prev = head;
        while (prev->next != tail) {
            prev = prev->next;
        }
        delete tail;
        tail = prev;
        tail->next = nullptr;
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
        Node* newNode = new Node(value, targetNode->next);
        targetNode->next = newNode;
        m_size++;
    }

    // Вставка до target
    void insert_before(const T& target, const T& value) {
        if (empty()) {
            throw std::runtime_error("Target element not found");
        }
        if (head->data == target) {
            push_front(value);
            return;
        }
        Node* prev = head;
        while (prev->next != nullptr && prev->next->data != target) {
            prev = prev->next;
        }
        if (prev->next == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        Node* newNode = new Node(value, prev->next);
        prev->next = newNode;
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
        Node* toDelete = targetNode->next;
        targetNode->next = toDelete->next;
        if (toDelete == tail) {
            tail = targetNode;
        }
        delete toDelete;
        m_size--;
    }

    // Удаление до target
    void remove_before(const T& target) {
        if (empty() || head->data == target) {
            throw std::runtime_error("No element exists before target");
        }
        if (head->next != nullptr && head->next->data == target) {
            pop_front();
            return;
        }
        Node* prevPrev = head;
        while (prevPrev->next != nullptr && 
               prevPrev->next->next != nullptr && 
               prevPrev->next->next->data != target) {
            prevPrev = prevPrev->next;
        }
        if (prevPrev->next == nullptr || prevPrev->next->next == nullptr) {
            throw std::runtime_error("Target element not found or has no element before it");
        }
        Node* toDelete = prevPrev->next;
        prevPrev->next = toDelete->next;
        delete toDelete;
        m_size--;
    }

    // Удаление элемента по значению
    void remove_by_value(const T& target) {
        if (empty()) {
            throw std::runtime_error("Target element not found");
        }
        if (head->data == target) {
            pop_front();
            return;
        }
        Node* prev = head;
        while (prev->next != nullptr && prev->next->data != target) {
            prev = prev->next;
        }
        if (prev->next == nullptr) {
            throw std::runtime_error("Target element not found");
        }
        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        if (toDelete == tail) tail = prev;
        delete toDelete;
        m_size--;
    }

    T& front() {
        if (empty()) throw std::runtime_error("ForwardList is empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("ForwardList is empty");
        return head->data;
    }

    T& back() {
        if (empty()) throw std::runtime_error("ForwardList is empty");
        return tail->data;
    }

    const T& back() const {
        if (empty()) throw std::runtime_error("ForwardList is empty");
        return tail->data;
    }

    T& at(size_t index) {
        if (index >= m_size) throw std::runtime_error("Index out of bounds");
        Node* curr = head;
        for (size_t i = 0; i < index; ++i) curr = curr->next;
        return curr->data;
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