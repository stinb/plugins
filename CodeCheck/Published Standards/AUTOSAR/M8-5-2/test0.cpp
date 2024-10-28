// MISRA08_8-5-2 and M8-5-2

#include <cstdint>

// The following are compliant
int16_t a0[5] = { 0 };                                              // Zero initialization
int16_t a1[2][2] = { };                                             // Zero initialization
int16_t a2[5] = { 1, 2, 3, 0, 0 };                                  // Non-zero initialization
int16_t a3[3][2] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };                // 2 dimensions
int16_t a4[3][1][2] = { { { 1, 2 } }, { { 3, 4 } }, { { 5, 6 } } }; // More than 2 dimensions
int16_t a5[1][3][2] = { { { 0, 1 }, { 0, 1 }, { 0, 1 } } };         // More than 2 dimensions

// The following are non-compliant
int16_t b0[5] = { 1, 2, 3 };                                         // UndCC_Violation
int16_t b1[3][2] = { 1, 2, 3, 4, 5, 6 };                             // UndCC_Violation
int16_t b2[2][2] = { { }, { 1, 2 } };                                // UndCC_Violation
int16_t b3[3][1][2] = { {{ 1, 2 }}, {{ 0 }}, {{ 5, 6 }} };           // UndCC_Violation
