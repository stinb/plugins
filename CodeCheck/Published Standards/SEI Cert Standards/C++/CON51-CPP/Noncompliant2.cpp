#include <mutex>

void manipulate_shared_data(std::mutex &pm)
{
  pm.lock(); // UndCC_Violation
  try {
    // Perform work on shared data.
  } catch (...) {
    pm.unlock();
    throw;
  }

  // missing unlock here in case no exceptions occur
}
