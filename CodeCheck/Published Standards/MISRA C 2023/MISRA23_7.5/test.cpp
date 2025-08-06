#include <stdint.h>
uint32_t u1 = UINT32_C( 10 ); /* UndCC_Valid */
//uint32_t u2 = UINT32_C( 10UL ); /* Violation: commented out due to analysis error - constant is suffixed */
//uint32_t u3 = UINT32_C( 10.0 ); /* Violation: commented out due to analysis error - floating-point constant */
uint16_t u4 = UINT16_C( -2 ); /* UndCC_Violation - constant expression */
int32_t s1 = INT32_C( -2 ); /* UndCC_Violation - constant expression */
int32_t s2 = -INT32_C( 2 ); /* UndCC_Valid */
uint_least16_t u5 = UINT16_C ( 0x10000 ); /* UndCC_FalseNeg  even if uint_least16_t
                                          ... is implemented as a 32-bit type */

