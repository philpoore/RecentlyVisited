#pragma once

#include <cassert>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>

template <typename K, typename V> class LinkedHashMap {

    struct Node {
        Node *prev = nullptr;
        Node *next = nullptr;
        V v;
        K k;
    };

    Node *head = nullptr;
    Node *tail = nullptr;

    std::unordered_map<K, Node *> data;

  public:
    LinkedHashMap() {}
    ~LinkedHashMap() {
        for (auto &it : data)
            delete it.second;
    }

    void insert(K k, V v) {
        if (data.count(k)) {
            // Move to head if already in data
            Node *n = data[k];
            // Node is already at head, don't need to update pointers
            // just update value.
            if (n == head) {
                n->v = v;
                return;
            }

            if (n->next)
                n->next->prev = n->prev;
            if (n->prev)
                n->prev->next = n->next;

            if (head)
                head->prev = n;
            if (n == tail && tail)
                tail = tail->prev;

            n->v = v;
            n->next = head;
            n->prev = nullptr;

            head = n;
            return;
        }

        Node *n = new Node();
        n->k = k;

        n->v = v;
        n->prev = nullptr;
        n->next = head;

        if (head)
            head->prev = n;

        if (!tail)
            tail = n;

        head = n;
        data[k] = n;
    }

    bool has(K k) { return data.count(k) > 0; }
    V get(K k) { return data[k]->v; }
    void remove(K k) {
        if (!data.count(k))
            return;

        Node *n = data[k];
        if (n->next) {
            n->next->prev = n->prev;
        }
        if (n->prev) {
            n->prev->next = n->next;
        }
        if (head == n) {
            head = n->next;
        }
        if (tail == n) {
            tail = n->prev;
        }

        data.erase(k);

        delete n;
    }

    int size() { return data.size(); }

    void pop_back() {
        if (!tail) {
            return;
        }

        Node *n = tail;
        tail = tail->prev;
        if (tail)
            tail->next = nullptr;

        data.erase(n->k);
        delete n;
    }

    std::vector<K> keys() {
        std::vector<K> o;
        Node *n = head;
        while (n) {
            o.push_back(n->k);
            n = n->next;
        }
        return o;
    }

    std::vector<V> values() {
        std::vector<V> o;
        Node *n = head;
        while (n) {
            o.push_back(n->v);
            n = n->next;
        }
        return o;
    }

    void debug() {
        int before = data.size();
        std::cout << "\n==== DEBUG ====\n";
        std::cout << "head: " << head << "\n";
        std::cout << "tail: " << tail << "\n";
        std::cout << "size: " << data.size() << "\n";
        std::cout << "data:\n";
        for (auto &a : data) {
            std::cout << a.second << " " << a.first << ":" << a.second->v
                      << " ";
        }
        std::cout << "\n";

        std::cout << "head:\n";
        Node *a = head;
        while (a) {
            std::cout << a << " " << a->k << ":" << a->v << " ";
            a = a->next;
        }
        std::cout << "\n";

        std::cout << "tail:\n";
        a = tail;
        while (a) {
            std::cout << a << " " << a->k << ":" << a->v << " ";
            a = a->prev;
        }
        std::cout << "\n";

        std::cout << "\n---- DEBUG ----\n\n\n\n";
        std::flush(std::cout);
    }

    class iterator;
    class reverse_iterator;

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }

    reverse_iterator rbegin() { return reverse_iterator(tail); }
    reverse_iterator rend() { return reverse_iterator(nullptr); }

    class iterator {
      public:
        using self_type = iterator;
        using value_type = V;
        using reference = V &;
        using pointer = Node *;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = int;
        iterator(pointer ptr) : ptr_(ptr) {}
        self_type operator++() {
            self_type i = *this;
            ptr_ = ptr_->next;
            return i;
        }
        const self_type operator++(int /* junk */) {
            ptr_ = ptr_->next;
            return *this;
        }
        reference operator*() { return ptr_->v; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type &rhs) { return ptr_ != rhs.ptr_; }

      private:
        pointer ptr_ = nullptr;
    };

    class reverse_iterator {
      public:
        using self_type = reverse_iterator;
        using value_type = V;
        using reference = V &;
        using pointer = Node *;
        using iterator_category = std::reverse_iterator<iterator>;
        using difference_type = int;
        reverse_iterator(pointer ptr) : ptr_(ptr) {}
        self_type operator++() {
            self_type i = *this;
            ptr_ = ptr_->prev;
            return i;
        }
        const self_type operator++(int /* junk */) {
            ptr_ = ptr_->prev;
            return *this;
        }
        reference operator*() { return ptr_->v; }
        pointer operator->() { return ptr_; }
        bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
        bool operator!=(const self_type &rhs) { return ptr_ != rhs.ptr_; }

      private:
        pointer ptr_ = nullptr;
    };
};