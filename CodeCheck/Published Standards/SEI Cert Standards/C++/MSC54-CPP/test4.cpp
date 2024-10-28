// Custom

#include <csignal>
#include <cstdlib>
#include <math.h>

class Virtual {
public:
  virtual void fn();
};

class Base : virtual Virtual {
public:
  void fn();
};

struct Point {
  int x;
  int y;
};

void no_extern_c(int sig)
{

}

extern "C" void standard_library_fn(int sig)
{
  rand(); // UndCC_Violation(Win) - standard library function
}

extern "C" void other_standard_library_fn(int sig)
{
  sin(0); // UndCC_Violation(Win) - standard library function
}

extern "C" void standard_library_fn_caller(int sig)
{
  other_standard_library_fn(sig);
}

extern "C" void standard_library_fn_caller_caller(int sig)
{
  standard_library_fn_caller(sig);
}

extern "C" void reinterpret_cast_expr(int sig)
{
  void *ptr = nullptr;                       // UndCC_Violation(Win) - C++ only feature
  reinterpret_cast<Base *>((Virtual *) ptr); // UndCC_Violation(Win) - C++ only feature
}

extern "C" void dynamic_cast_expr(int sig)
{
  void *ptr = nullptr;                   // UndCC_Violation(Win) - C++ only feature
  dynamic_cast<Base *>((Virtual *) ptr); // UndCC_Violation(Win) - C++ only feature
}

extern "C" void throw_exception(int sig)
{
  throw 0; // UndCC_Violation(Win) - C++ only feature
}

extern "C" void constructor(int sig)
{
  Point p; // UndCC_Violation(Win) - C++ only feature
}

void install_signal_handler()
{
  std::signal(SIGTERM, no_extern_c); // UndCC_Violation(Win) - missing C linkage

  std::signal(SIGTERM, standard_library_fn);
  std::signal(SIGTERM, standard_library_fn_caller);
  std::signal(SIGTERM, standard_library_fn_caller_caller);

  std::signal(SIGTERM, reinterpret_cast_expr);
  std::signal(SIGTERM, dynamic_cast_expr);

  std::signal(SIGTERM, throw_exception);
  std::signal(SIGTERM, constructor);
}
