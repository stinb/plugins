#include <cstdint>

// Helper functions for aliasing tests
void modify_ref(int32_t &x) { x = 0; }
void read_ref(const int32_t &x) { (void)x; }
void modify_ptr(int32_t *p) { *p = 0; }

// ===== Compliant cases =====

// Basic increment loop
void compliant_basic_increment()
{
    for (int32_t i = 0; i < 10; ++i) {} // UndCC_Valid
}

// Basic decrement loop
void compliant_basic_decrement()
{
    for (int32_t i = 10; i > 0; --i) {} // UndCC_Valid
}

// Post-increment
void compliant_post_increment()
{
    for (int32_t i = 0; i < 10; i++) {} // UndCC_Valid
}

// Variable bound, same type
void compliant_variable_bound(int32_t n)
{
    for (int32_t i = 0; i < n; ++i) {} // UndCC_Valid
}

// Compound step +=
void compliant_compound_step()
{
    for (int32_t i = 0; i < 100; i += 2) {} // UndCC_Valid
}

// Compound step -=
void compliant_compound_step_minus()
{
    for (int32_t i = 100; i > 0; i -= 5) {} // UndCC_Valid
}

// Assignment step: i = i + step
void compliant_assignment_step()
{
    for (int32_t i = 0; i < 100; i = i + 3) {} // UndCC_Valid
}

// Assignment step: i = i - step
void compliant_assignment_step_minus()
{
    for (int32_t i = 100; i > 0; i = i - 3) {} // UndCC_Valid
}

// Constexpr bound with different type that fits
void compliant_constexpr_bound_fits()
{
    for (int32_t i = 0; i < 10L; ++i) {} // UndCC_Valid - 10L fits in int32_t
}

// Counter on RHS of condition
void compliant_counter_rhs()
{
    for (int32_t i = 0; 10 > i; ++i) {} // UndCC_Valid
}

// Unsigned counter
void compliant_unsigned()
{
    for (uint32_t i = 0U; i < 10U; ++i) {} // UndCC_Valid
}

// Nested loops
void compliant_nested()
{
    for (int32_t i = 0; i < 10; ++i) { // UndCC_Valid
        for (int32_t j = 0; j < 10; ++j) {} // UndCC_Valid
    }
}

// Const ref to counter in body is OK
void compliant_const_ref_in_body()
{
    for (int32_t i = 0; i < 10; ++i) { // UndCC_Valid
        read_ref(i);
    }
}

// Auto-deduced counter type (int literal -> int)
void compliant_auto_deduced()
{
    for (auto i = 0; i < 10; ++i) {} // UndCC_Valid
}

// Greater-equal operator
void compliant_ge_operator()
{
    for (int32_t i = 10; i >= 0; --i) {} // UndCC_Valid
}

// Less-equal operator
void compliant_le_operator()
{
    for (int32_t i = 0; i <= 10; ++i) {} // UndCC_Valid
}

// Step is commutative: i = step + i
void compliant_step_commutative()
{
    for (int32_t i = 0; i < 100; i = 3 + i) {} // UndCC_Valid
}

// ===== Non-compliant: C1 - init-statement violations =====

// No init-statement
void nc_c1_no_init()
{
    int32_t i = 0;
    for (; i < 10; ++i) {} // UndCC_Violation - no init-statement
}

// Init is expression, not declaration
void nc_c1_expr_init()
{
    int32_t i;
    for (i = 0; i < 10; ++i) {} // UndCC_Violation - init is expression not declaration
}

// Multiple declarations
void nc_c1_multiple_decls()
{
    for (int32_t i = 0, j = 0; i < 10; ++i) {} // UndCC_Violation - multiple declarations
}

// Float counter
void nc_c1_float_counter()
{
    for (float f = 0.0f; f < 1.0f; f += 0.1f) {} // UndCC_Violation - float counter
}

// Double counter
void nc_c1_double_counter()
{
    for (double d = 0.0; d < 10.0; d += 1.0) {} // UndCC_Violation - double counter
}

// Pointer counter
void nc_c1_pointer_counter()
{
    int32_t arr[10];
    for (int32_t *p = arr; p < arr + 10; ++p) {} // UndCC_Violation - pointer counter
}

// Bool counter
void nc_c1_bool_counter()
{
    for (bool b = false; b < true; b = true) {} // UndCC_Violation - bool counter
}

// Uninitialized counter
void nc_c1_no_initializer()
{
    for (int32_t i; i < 10; ++i) {} // UndCC_Violation - no initializer
}

// ===== Non-compliant: C2 - condition violations =====

// No condition
void nc_c2_no_condition()
{
    for (int32_t i = 0; ; ++i) { break; } // UndCC_Violation - no condition
}

// Not-equal operator
void nc_c2_ne_operator()
{
    for (int32_t i = 0; i != 10; ++i) {} // UndCC_Violation - != is not relational
}

// Equality operator
void nc_c2_eq_operator()
{
    for (int32_t i = 0; i == 0; ++i) {} // UndCC_Violation - == is not relational
}

// Counter not in condition
void nc_c2_counter_not_in_cond()
{
    int32_t x = 10;
    for (int32_t i = 0; x > 0; ++i) {} // UndCC_Violation - counter not in condition
}

// Complex condition (&&)
void nc_c2_complex_condition()
{
    for (int32_t i = 0; i < 10 && i > 0; ++i) {} // UndCC_Violation - && is not a relational op
}

// ===== Non-compliant: C3 - increment violations =====

// No increment
void nc_c3_no_increment()
{
    for (int32_t i = 0; i < 10; ) { break; } // UndCC_Violation - no increment
}

// Multiply step
void nc_c3_multiply_step()
{
    for (int32_t i = 1; i < 1000; i *= 2) {} // UndCC_Violation - *= is not add/subtract
}

// Shift step
void nc_c3_shift_step()
{
    for (int32_t i = 1; i < 1000; i <<= 1) {} // UndCC_Violation - <<= is not add/subtract
}

// Modulo step
void nc_c3_modulo_step()
{
    for (int32_t i = 1; i < 1000; i %= 10) {} // UndCC_Violation - %= is not add/subtract
}

// ===== Non-compliant: C4 - type mismatch =====

// Type mismatch with non-constexpr bound
void nc_c4_type_mismatch(int64_t n)
{
    for (int32_t i = 0; i < n; ++i) {} // UndCC_Violation - int64_t bound, non-constexpr
}

// ===== Non-compliant: C5 - bound/step modified in body =====

// Bound modified in body
void nc_c5_bound_modified(int32_t n)
{
    for (int32_t i = 0; i < n; ++i) { // UndCC_Violation - bound modified in body
        n--;
    }
}

// Step modified in body
void nc_c5_step_modified()
{
    int32_t step = 1;
    for (int32_t i = 0; i < 100; i += step) { // UndCC_Violation - step modified in body
        step++;
    }
}

// ===== Non-compliant: C6 - aliasing violations =====

// Counter passed by non-const ref
void nc_c6_counter_nonconst_ref()
{
    for (int32_t i = 0; i < 10; ++i) { // UndCC_Violation - counter passed by non-const ref
        modify_ref(i);
    }
}

// Counter address taken
void nc_c6_counter_addr_taken()
{
    for (int32_t i = 0; i < 10; ++i) { // UndCC_Violation - address of counter taken
        int32_t *p = &i;
        (void)p;
    }
}

// Bound passed by non-const ref
void nc_c6_bound_nonconst_ref(int32_t n)
{
    for (int32_t i = 0; i < n; ++i) { // UndCC_Violation - bound passed by non-const ref
        modify_ref(n);
    }
}

// Non-const ref variable created from counter
void nc_c6_nonconst_ref_var()
{
    for (int32_t i = 0; i < 10; ++i) { // UndCC_Violation - non-const ref to counter
        int32_t &ref = i;
        (void)ref;
    }
}
