void func0() {
  try {} catch (int e1) {};        // UndCC_Violation
  try {} catch (int const &e2) {}; // UndCC_Valid
  try {} catch (const int &e3) {}; // UndCC_Valid
  try {} catch (...) {};           // UndCC_Violation(strict)
}
