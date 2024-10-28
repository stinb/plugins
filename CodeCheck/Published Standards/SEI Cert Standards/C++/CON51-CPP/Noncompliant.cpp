#include <mutex>

void manipulate_shared_data(std::mutex &pm)
{
    pm.lock(); // UndCC_Violation

    // Perform work on shared data.

    pm.unlock();
}