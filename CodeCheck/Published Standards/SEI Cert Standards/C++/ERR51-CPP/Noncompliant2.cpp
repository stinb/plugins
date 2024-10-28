#include <thread>
#include <stdio.h>

void throwing_func4() noexcept(false) {}

void thread_start2()
{
    throwing_func4(); // UndCC_Violation
}

void f4()
{
    std::thread t(thread_start2);
    t.join();
}
