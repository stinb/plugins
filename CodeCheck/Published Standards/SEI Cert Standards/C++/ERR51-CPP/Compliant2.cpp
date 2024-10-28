#include <thread>

void throwing_func2() noexcept(false) {}

void thread_start(void)
{
    try
    {
        throwing_func2();
    }
    catch (...)
    {
        // Handle error
    }
}

void f2()
{
    std::thread t(thread_start);
    t.join();
}
