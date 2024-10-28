namespace A {}

namespace B {}

using namespace A; // UndCC_Violation
int main() {
  using namespace B; // UndCC_Valid
  return 0;
}
