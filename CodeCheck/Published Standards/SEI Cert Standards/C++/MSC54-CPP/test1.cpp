// SEI CERT C++

#include <csignal>

extern "C" void sig_handler(int sig) {
  // Implementation details elided.
}

void install_signal_handler() {
  if (SIG_ERR == std::signal(SIGTERM, sig_handler)) {
    // Handle error
  }
}
