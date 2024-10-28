// SEI CERT C++

#include <csignal>

volatile sig_atomic_t signal_flag = 0;
static void g() noexcept(false);

extern "C" void sig_handler(int sig) {
  signal_flag = 1;
}

void install_signal_handler() {
  if (SIG_ERR == std::signal(SIGTERM, sig_handler)) {
    // Handle error
  }
}

// Called periodically to poll the signal flag.
void poll_signal_flag() {
  if (signal_flag == 1) {
    signal_flag = 0;
    try {
      g();
    } catch(...) {
      // Handle error
    }
  }
}
