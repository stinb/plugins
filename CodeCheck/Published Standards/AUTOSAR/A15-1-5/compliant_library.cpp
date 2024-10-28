// SEI CERT C++

int func() noexcept(true) {
  // ...
  if (true) {
    return 42;
  }
  // ...
  return 0;
}
