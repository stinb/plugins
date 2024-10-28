#include <mutex>

void manipulate_shared_data(std::mutex &pm)
{
    pm.lock();
    try
    {
        // Perform work on shared data.
    }
    catch (...)
    {
        pm.unlock();
        throw;
    }
    pm.unlock(); // in case no exceptions occur
}