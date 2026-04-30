// Binary addition wrap
void test_addition() {
  unsigned int a = 0xFFFFFFFFu + 1u;   // UndCC_Violation
  unsigned int b = 10u + 20u;          // UndCC_Valid
}

// Binary subtraction wrap
void test_subtraction() {
  unsigned int a = 0u - 1u;            // UndCC_Violation
  unsigned int b = 5u - 10u;           // UndCC_Violation
  unsigned int c = 20u - 10u;          // UndCC_Valid
}

// Binary multiplication wrap
void test_multiplication() {
  unsigned int a = 100000u * 100000u;  // UndCC_Violation - 10^10 > UINT32_MAX
  unsigned int b = 100u * 100u;        // UndCC_Valid
}

// Left shift wrap
void test_shift() {
  unsigned int a = 1u << 33;           // UndCC_Violation - shift >= width
  unsigned int b = 1u << 31;           // UndCC_Valid
}

// Unary negation wrap
void test_negation() {
  unsigned int a = -1u;                // UndCC_Violation
  unsigned int b = -0u;                // UndCC_Valid
}

// Division and modulo cannot wrap
void test_div_mod() {
  unsigned int a = 100u / 3u;          // UndCC_Valid
  unsigned int b = 100u % 7u;          // UndCC_Valid
}

// Unevaluated contexts: constant ternary
void test_ternary_unevaluated() {
  // Condition is constant true: false-branch is unevaluated
  unsigned int a = (0u == 0u) ? 0u : (0u - 1u);   // UndCC_Valid

  // Condition is constant false: true-branch is unevaluated
  unsigned int b = (0u != 0u) ? (0u - 1u) : 0u;   // UndCC_Valid
}

// Non-constant ternary: both branches are evaluated
void test_ternary_evaluated(bool cond) {
  unsigned int a = cond ? 0u : (0u - 1u);          // UndCC_Violation
}

// Short-circuit logical AND
void test_short_circuit_and() {
  // LHS is constant false: RHS is unevaluated
  bool a = (false) && ((0u - 1u) > 0u);        // UndCC_Valid
}

// Short-circuit logical OR
void test_short_circuit_or() {
  // LHS is constant true: RHS is unevaluated
  bool a = (true) || ((0u - 1u) > 0u);         // UndCC_Valid
}

// sizeof is unevaluated context
void test_sizeof() {
  auto a = sizeof(0u - 1u);                    // UndCC_Valid
}

// Non-constant operands: rule does not apply
void test_non_constant(unsigned int x, unsigned int y) {
  unsigned int a = x + y;                      // UndCC_Valid
  unsigned int b = x + 100u;                   // UndCC_Valid
}

// constexpr function: parameters are not constant expressions inside
constexpr unsigned short add(unsigned short a, unsigned short b) {
  return a + b;                                 // UndCC_Valid
}

// Sub-expression wrapping: intermediate result wraps even if final is in range
void test_sub_expression() {
  unsigned int a = 10u - 20u + 15u;            // UndCC_Violation - (10u - 20u) wraps
  unsigned int b = 10u + 15u - 20u;            // UndCC_Valid
}

// Cast does not hide the wrapping sub-expression
void test_cast() {
  unsigned int a = static_cast<unsigned int>(0u - 1u); // UndCC_Violation
}

// Brace-initialization
void test_brace_init() {
  unsigned int a{ 0u - 1u };                   // UndCC_Violation
}

// constexpr variable references are constant expressions
void test_constexpr_vars() {
  constexpr unsigned int kMax = 0xFFFFFFFFu;
  unsigned int a = kMax + 1u;                  // UndCC_Violation
  unsigned int b = kMax - 1u;                  // UndCC_Valid
}

// Macro-expanded code is still checked
#define BIG 0xFFFFFFFFu
void test_macro() {
  unsigned int x = BIG + 1u;                   // UndCC_Violation
}
