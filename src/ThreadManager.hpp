#pragma once

#include <thread>
#include <chrono>
#include <atomic>

class ThreadManager {
private:
    ThreadManager() {}
    static std::shared_ptr<ThreadManager> instance;
public:
    std::shared_ptr<ThreadManager> get_instance() {

    }

};
