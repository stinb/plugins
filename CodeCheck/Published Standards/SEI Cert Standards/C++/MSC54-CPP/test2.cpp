// SEI CERT C++

#include <csignal>

static void g() noexcept(false);

extern "C" void sig_handler(int sig) {
  try { // UndCC_Violation(Win) - C++ only feature
    g();
  } catch (...) { // UndCC_Violation(Win) - C++ only feature
    // Handle error
  }
}

void install_signal_handler() {
  if (SIG_ERR == std::signal(SIGTERM, sig_handler)) {
    // Handle error
  }
}
