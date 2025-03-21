#include <mutex>
#include <thread>

using namespace std;

mutex m;

void do_thread_safe_work();

void do_work()
{
    while (!m.try_lock())
    {
        // The lock is not owned yet, do other work while waiting.
        do_thread_safe_work();
    }
    try
    {
        // The mutex is now locked; perform work on shared resources.
        // ...

        // Release the mutex.
    }
    catch (...)
    {
        m.unlock();
        throw;
    }
    m.unlock();
}

void start_func()
{
    do_work();
}

int main()
{
    std::thread t(start_func);

    do_work();

    t.join();
}