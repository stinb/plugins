// SEI CERT C++

#include <mutex>

static std::mutex m;
static int shared_resource;

void increment_by_42() {
  std::unique_lock<std::mutex>(m); // UndCC_Violation
  shared_resource += 42;
}
