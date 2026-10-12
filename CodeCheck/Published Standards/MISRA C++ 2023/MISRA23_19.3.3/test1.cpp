#include <cstdint>
#include "scale.h"

#define HAA HBB

std::int32_t hspeed;
std::int32_t hspeed_scale;
std::int32_t HAA_scale = 1;
std::int32_t HBB = 42;
std::int32_t HBB_scale = 2;

/* The macro is defined in a header rather than this file. */
std::int32_t ok = HSCALE( hspeed );     /* UndCC_Valid */
std::int32_t bad = HSCALE( HAA );       /* UndCC_Violation */
