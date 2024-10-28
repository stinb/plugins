#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

constexpr size_t numThreads = 5;

std::mutex mutex;
std::condition_variable cond[numThreads];

void run_step(size_t myStep)
{
    static size_t currentStep = 0;
    std::unique_lock<std::mutex> lk(mutex);

    std::cout << "Thread " << myStep << " has the lock" << std::endl;

    while (currentStep != myStep)
    {
        std::cout << "Thread " << myStep << " is sleeping..." << std::endl;
        cond[myStep].wait(lk);
        std::cout << "Thread " << myStep << " woke up" << std::endl;
    }

    // Do processing ...
    std::cout << "Thread " << myStep << " is processing..." << std::endl;
    currentStep++;

    // Signal next step thread.
    if ((myStep + 1) < numThreads)
    {
        cond[myStep + 1].notify_one();
    }

    std::cout << "Thread " << myStep << " is exiting..." << std::endl;
}

// ... main() unchanged ...