// Custom

#include <cstring>
#include <iostream>

struct S0 {
private:
  int scalingFactor;
  int otherData;

public:
  S0() : scalingFactor(1) {}

  void set_other_data(int i);
  int f(int i)
  {
    return i / scalingFactor;
  }
  // ...
};

void f0()
{
  S0 a;

  // ... Code that mutates a ...

  // Reinitialize a to its default state
  std::memset(&a, 0, sizeof(S0)); // UndCC_Violation

  std::cout << a.f(100) << std::endl;
}

struct S1 {
private:
  int *i;

public:
  S1() : i(nullptr) {}
  ~S1() { delete i; }

  void set(int val)
  {
    if (i) {
      delete i;
    }
    i = new int{val};
  }

  // ...
};

void f1(S1 &a)
{
  S1 b;
  std::memcpy(&b, &a, sizeof(S1)); // UndCC_Violation
}

struct S2
{
private:
  int i;

public:
  virtual void f();

  // ...
};

void f(S2 &a, S2 &b)
{
  if (!std::memcmp(&a, &b, sizeof(S2))) { // UndCC_Violation
    // ...
  }
}
