#pragma once

#include "LinkedHashMap.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

template <typename _K = std::string, typename _V = std::string,
          uint64_t C = 100>
class LRU_LinkedHashMap {
    typedef _K K;
    typedef _V V;

    int capacity = C;
    LinkedHashMap<K, V> data;

  public:
    void set(K k, V v) {
        data.insert(k, v);
        while (data.size() > C) {
            data.pop_back();
        }
    }

    V get(K k) { return data.get(k); }

    void del(K k) { data.remove(k); }

    bool has(K k) { return data.has(k); }

    int size() { return data.size(); }

    std::vector<K> keys() { return data.keys(); }

    std::vector<V> values() { return data.values(); }
};