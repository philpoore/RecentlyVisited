#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename Args> class WorkerPool {
    typedef void(F)(Args);
    int size;
    std::vector<std::thread *> threads;
    F *f;
    std::queue<Args *> q;

    std::mutex m;
    std::condition_variable cv;
    bool _terminate = false;

    void loop() {
        std::cout << "Starting worker...\n";
        while (true) {
            Args task;
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
            {
                std::unique_lock<std::mutex> lk(m);

                if (!q.size()) {
                    std::cout << std::this_thread::get_id()
                              << " worker waiting\n";
                    cv.wait(lk, [&] { return q.size() || _terminate; });
                    std::cout << std::this_thread::get_id()
                              << " worker waited\n";
                }

                if (_terminate)
                    break;

                if (!q.size())
                    continue;

                task = *q.front();

                delete q.front();
                q.pop();
            }

            f(task);
        }
        std::cout << "Worker exited...\n";
    }

  public:
    WorkerPool(int size, WorkerPool::F *f) : size(size), f(f) {
        for (int i = 0; i < size; i++) {
            threads.push_back(new std::thread(&WorkerPool::loop, this));
        }
    }

    ~WorkerPool() { join(); }

    void add(Args *args) {
        {
            std::unique_lock<std::mutex> lk(m);
            q.push(args);
        }
        cv.notify_one();
    }

    void terminate() {
        _terminate = true;
        {
            std::unique_lock<std::mutex> lk(m);
            cv.notify_all();
        }
    }

    void join() {
        {
            std::unique_lock<std::mutex> lk(m);
            _terminate = true;
        }
        cv.notify_all();

        for (auto t : threads) {
            t->join();
            delete t;
        }
        std::cout << "All done\n";
    }
};