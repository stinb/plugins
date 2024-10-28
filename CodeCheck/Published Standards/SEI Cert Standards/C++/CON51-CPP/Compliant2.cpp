#include <mutex>

void manipulate_shared_data(std::mutex &pm)
{
    std::lock_guard<std::mutex> lk(pm);

    // Perform work on shared data.
}