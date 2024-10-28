#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex;
std::condition_variable cond;

void run_step(size_t myStep)
{
    static size_t currentStep = 0;
    std::unique_lock<std::mutex> lk(mutex);

    std::cout << "Thread " << myStep << " has the lock" << std::endl;

    while (currentStep != myStep)
    {
        std::cout << "Thread " << myStep << " is sleeping..." << std::endl;
        cond.wait(lk);
        std::cout << "Thread " << myStep << " woke up" << std::endl;
    }

    // Do processing ...
    std::cout << "Thread " << myStep << " is processing..." << std::endl;
    currentStep++;

    // Signal ALL waiting tasks.
    cond.notify_all();

    std::cout << "Thread " << myStep << " is exiting..." << std::endl;
}

// ... main() unchanged ...