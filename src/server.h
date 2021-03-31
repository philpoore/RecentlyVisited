#pragma once

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <map>
#include <mutex>
#include <signal.h>
#include <string>
#include <thread>
#include <unordered_map>

#include "LRU.h"
#include "socket.h"
#include "worker_pool.h"

typedef std::uint64_t ll;

struct Args {
    int client_fd;
    struct sockaddr client_addr;
    Args() = default;
    Args(int client_fd, struct sockaddr client_addr)
        : client_fd(client_fd), client_addr(client_addr) {}
};

template <typename K, typename V> struct Record {
    std::mutex m;
    LRU_LinkedHashMap<K, V, 10> lru;

    void set(K k, V v) {
        std::lock_guard<std::mutex> lg(m);
        lru.set(k, v);
    }

    std::vector<ll> keys() { return lru.keys(); }
};

class RecentlyVisited {
  public:
    std::unordered_map<std::string, Record<ll, ll>> data;
};