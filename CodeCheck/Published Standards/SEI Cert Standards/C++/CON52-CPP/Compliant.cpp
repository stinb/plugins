#include <mutex>

struct MultiThreadedFlags
{
    unsigned int flag1 : 2;
    unsigned int flag2 : 2;
};

struct MtfMutex
{
    MultiThreadedFlags s;
    std::mutex mutex;
};

MtfMutex flags;

void thread1()
{
    std::lock_guard<std::mutex> lk(flags.mutex);
    flags.s.flag1 = 1;
}

void thread2()
{
    std::lock_guard<std::mutex> lk(flags.mutex);
    flags.s.flag2 = 2;
}