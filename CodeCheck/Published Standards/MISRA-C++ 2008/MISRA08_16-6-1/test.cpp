// MISRA C++ 2008 - 16-6-1

#pragma once   // UndCC_Violation

// Pack structures to the byte boundary for the wire protocol
#pragma pack(push, 1)   // UndCC_Valid - documented on the preceding line

#pragma pack(pop)  /* restores the previous packing */  // UndCC_Valid - documented on the same line

/*
 * Suppress the vendor warning about the interrupt attribute.
 */
#pragma warning(disable : 4001)   // UndCC_Valid - documented by a block comment

#pragma GCC diagnostic push   // UndCC_Violation

#define NOT_A_PRAGMA 1   // UndCC_Valid - not a pragma
#include <cstdint>       // UndCC_Valid - not a pragma

void f(void)
{
#pragma omp barrier   // UndCC_Violation - inside a function body
}

#if defined(_MSC_VER)
#pragma warning(push)   // UndCC_Violation - inactive code is still checked
// disable the padding warning for the wire structs
#pragma warning(disable : 4820)   // UndCC_Valid
#endif

int x;   // sets up x
#pragma three   // UndCC_Violation - the comment above documents x, not the pragma

// documented, but a blank line intervenes

#pragma four   // UndCC_Violation

// documented
#  pragma five   // UndCC_Valid - indented directive

// documented
#pragma six     // UndCC_Valid
#pragma seven   // UndCC_Violation - a pragma is not a comment

#define PUSH_WARNINGS _Pragma("warning(push)")   // UndCC_Valid - _Pragma is not the #pragma directive

#define PUSH #pragma warning(push)   // UndCC_Violation - a use of the directive
