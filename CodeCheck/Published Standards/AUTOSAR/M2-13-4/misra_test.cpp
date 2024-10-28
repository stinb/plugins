#include <cstdint>

int64_t const a = 0L;       // UndCC_Valid
int64_t const b = 0l;       // UndCC_Violation
uint64_t const c = 1Lu;     // UndCC_FalsePos(0)
uint64_t const d = 1lU;     // UndCC_Violation
uint64_t const e = 2uLL;    // UndCC_FalsePos(0) 
uint64_t const f = 2Ull;    // UndCC_FalsePos(0) 
long long const g = 3LL;    // UndCC_Valid
long long const h = 3ll;    // UndCC_Violation
long double const i = 1.2L; // UndCC_Valid
long double const j = 3.4l; // UndCC_Violation

class Litre
{
};

constexpr Litre operator"" _l(long double val) noexcept
{
    return Litre{}; // Assumes type Litre is defined
}

auto volume = 42.1_l; // Rule does not apply
