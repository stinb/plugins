#include <cstdint>

// Binary addition wrap
void test_addition() {
  uint32_t a = 0xFFFFFFFFu + 1u;   // UndCC_Violation
  uint32_t b = 10u + 20u;          // UndCC_Valid
}

// Binary subtraction wrap
void test_subtraction() {
  uint32_t a = 0u - 1u;            // UndCC_Violation
  uint32_t b = 5u - 10u;           // UndCC_Violation
  uint32_t c = 20u - 10u;          // UndCC_Valid
}

// Binary multiplication wrap
void test_multiplication() {
  uint32_t a = 100000u * 100000u;  // UndCC_Violation - 10^10 > UINT32_MAX
  uint32_t b = 100u * 100u;        // UndCC_Valid
}

// Left shift wrap
void test_shift() {
  uint32_t a = 1u << 33;           // UndCC_Violation - shift >= width
  uint32_t b = 1u << 31;           // UndCC_Valid
}

// Unary negation wrap
void test_negation() {
  uint32_t a = -1u;                // UndCC_Violation
  uint32_t b = -0u;                // UndCC_Valid
}

// Division and modulo cannot wrap
void test_div_mod() {
  uint32_t a = 100u / 3u;          // UndCC_Valid
  uint32_t b = 100u % 7u;          // UndCC_Valid
}

// Unevaluated contexts: constant ternary
void test_ternary_unevaluated() {
  // Condition is constant true: false-branch is unevaluated
  uint32_t a = (0u == 0u) ? 0u : (0u - 1u);   // UndCC_Valid

  // Condition is constant false: true-branch is unevaluated
  uint32_t b = (0u != 0u) ? (0u - 1u) : 0u;   // UndCC_Valid
}

// Non-constant ternary: both branches are evaluated
void test_ternary_evaluated(bool cond) {
  uint32_t a = cond ? 0u : (0u - 1u);          // UndCC_Violation
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
void test_non_constant(uint32_t x, uint32_t y) {
  uint32_t a = x + y;                          // UndCC_Valid
  uint32_t b = x + 100u;                       // UndCC_Valid
}

// constexpr function: parameters are not constant expressions inside
constexpr uint16_t add(uint16_t a, uint16_t b) {
  return a + b;                                 // UndCC_Valid
}

// Sub-expression wrapping: intermediate result wraps even if final is in range
void test_sub_expression() {
  uint32_t a = 10u - 20u + 15u;               // UndCC_Violation - (10u - 20u) wraps
  uint32_t b = 10u + 15u - 20u;               // UndCC_Valid
}

// Cast does not hide the wrapping sub-expression
void test_cast() {
  uint32_t a = static_cast<uint32_t>(0u - 1u); // UndCC_Violation
}

// Brace-initialization
void test_brace_init() {
  uint32_t a{ 0u - 1u };                       // UndCC_Violation
}

// constexpr variable references are constant expressions
void test_constexpr_vars() {
  constexpr uint32_t kMax = 0xFFFFFFFFu;
  uint32_t a = kMax + 1u;                      // UndCC_Violation
  uint32_t b = kMax - 1u;                      // UndCC_Valid
}

// Macro-expanded code is still checked
#define BIG 0xFFFFFFFFu
void test_macro() {
  uint32_t x = BIG + 1u;                       // UndCC_Violation
}
