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

    // Do processing...
    std::cout << "Thread " << myStep << " is processing..." << std::endl;
    currentStep++;

    // Signal awaiting task.
    cond.notify_one();  // UndCC_Violation

    std::cout << "Thread " << myStep << " is exiting..." << std::endl;
}

int main()
{
    constexpr size_t numThreads = 5;
    std::thread threads[numThreads];

    // Create threads.
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads[i] = std::thread(run_step, i);
    }

    // Wait for all threads to complete.
    for (size_t i = numThreads; i != 0; --i)
    {
        threads[i - 1].join();
    }
}
