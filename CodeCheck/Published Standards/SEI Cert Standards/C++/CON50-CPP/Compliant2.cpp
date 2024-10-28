#include <mutex>
#include <thread>

const size_t maxThreads = 10;

void do_work(size_t i, std::mutex *pm)
{
    std::lock_guard<std::mutex> lk(*pm);

    // Access data protected by the lock.
}
void run_threads()
{
    std::thread threads[maxThreads];
    std::mutex m;

    for (size_t i = 0; i < maxThreads; ++i)
    {
        threads[i] = std::thread(do_work, i, &m);
    }

    for (size_t i = 0; i < maxThreads; ++i)
    {
        threads[i].join();
    }
}