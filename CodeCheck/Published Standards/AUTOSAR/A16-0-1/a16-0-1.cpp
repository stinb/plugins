// $Id: A16-0-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $


#ifndef HEADER_FILE_NAME // UndCC_Valid - include guard
#define HEADER_FILE_NAME // UndCC_Valid - include guard

#include <cstdint> // UndCC_Valid - unconditional file inclusion

#ifdef WIN32
#include <windows.h> // UndCC_Valid - conditional file inclusion
#endif

#ifdef WIN32 // UndCC_Violation
std::int32_t fn1(
    std::int16_t x,
    std::int16_t y) noexcept;  
#endif // UndCC_Violation

#if defined VERSION && VERSION > 2011L // UndCC_Valid
#include <array>                       // UndCC_Valid - conditional file inclusion
#elif VERSION > 1998L                  // UndCC_Valid
#include <vector>                      // UndCC_Valid - conditional file inclusion
#endif                                 // UndCC_Valid

#define MAX_ARRAY_SIZE 1024U // UndCC_Violation
#ifndef MAX_ARRAY_SIZE       // UndCC_Violation
#error "MAX_ARRAY_SIZE has not been defined" 
#endif                // UndCC_Violation
#undef MAX_ARRAY_SIZE // UndCC_Violation

#define MIN(a, b) (((a) < (b)) ? (a) : (b)) // UndCC_Violation
#define PLUS2(X) ((X) + 2)                  // UndCC_Violation - function should be used instead
#define PI 3.14159F                         // UndCC_Violation - constexpr should be used instead
#define std ::int32_t long                  // UndCC_Violation - ’using’ should be used instead
#define STARTIF if( // UndCC_Violation - language redefinition
#define HEADER "filename.h" // UndCC_Violation - string literal

void Fn2() noexcept
{
#ifdef __linux__ // UndCC_Violation - ifdef not used for file inclusion

    // ...

#elif WIN32 // UndCC_Violation - elif not used for file inclusion

    // ...

#elif __APPLE__ // UndCC_Violation - elif not used for file inclusion

    // ...

#else // UndCC_Violation - else not used for file inclusion

    // ...

#endif // UndCC_Violation - endif not used for file inclusion or include guards
}

#endif // UndCC_Valid - include guard
