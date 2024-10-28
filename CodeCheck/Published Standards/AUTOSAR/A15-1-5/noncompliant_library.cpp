// SEI CERT C++

void func() noexcept(false) {
  // ...
  if (true) {
    throw 42;
  }
}
