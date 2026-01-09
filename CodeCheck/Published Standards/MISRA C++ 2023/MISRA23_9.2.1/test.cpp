#include <mutex>

std::mutex a_mutex;
std::mutex b_mutex;
std::mutex other_mutex;

void f(std::unique_lock<std::mutex> lk)
{
    (void)lk;
}

void f1()
{
    std::unique_lock<std::mutex> a_lock;   // Declaration, rule does not apply
    std::unique_lock<std::mutex>(b_mutex); // Declaration, rule does not apply
}

void f2()
{
    std::scoped_lock{a_mutex}; // UndCC_Violation - locks and unlocks here Unprotected
}

void f3()
{
    std::scoped_lock(a_mutex, other_mutex); // UndCC_Violation - locks and unlocks here Unprotected
}

void f4()
{
    f(std::unique_lock<std::mutex>{a_mutex}); // UndCC_Valid - type conversion is
                                              // not an expression statement
}
