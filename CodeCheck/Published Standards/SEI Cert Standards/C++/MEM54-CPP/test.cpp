#include <new>
#include <type_traits>
#include <cstddef>
#include <algorithm>
#include <cstdlib>

// Global placement operator new[] used by compliant3::f(), which passes an
// explicit buffer size alongside the storage pointer.
void *operator new[](std::size_t n, void *p, std::size_t bufsize)
{
    if (n > bufsize)
    {
        throw std::bad_array_new_length();
    }
    return p;
}

namespace compliant1
{
void f()
{
    char c; // Used elsewhere in the function
    alignas(long) unsigned char buffer[sizeof(long)];
    long *lp = ::new (buffer) long; // UndCC_Valid

    // ...
}
}

namespace compliant2
{
void f()
{
    char c; // Used elsewhere in the function
    std::aligned_storage<sizeof(long), alignof(long)>::type buffer;
    long *lp = ::new (&buffer) long; // UndCC_Valid

    // ...
}
}

namespace compliant3
{
#if defined(__clang__) || defined(__GNUG__)
const size_t overhead = sizeof(size_t);
#else
static_assert(false, "you need to determine the size of your implementation's array overhead");
const size_t overhead = 0; // Declaration prevents additional diagnostics about overhead being undefined; the value used does not matter.
#endif

struct S
{
    S();
    ~S();
};

void f()
{
    const size_t n = 32;
    alignas(S) unsigned char buffer[sizeof(S) * n + std::max(overhead, alignof(S))];
    S *sp = ::new (buffer, sizeof(buffer)) S[n]; // UndCC_Valid

    // ...
    // Destroy elements of the array.
    for (size_t i = 0; i != n; ++i)
    {
        sp[i].~S();
    }
}
}

namespace compliant4
{
void f()
{
    long storage;
    long *lp = ::new (&storage) long; // UndCC_Valid

    // ...
}
}

namespace compliant5
{
void f()
{
    void *buffer = std::aligned_alloc(alignof(long), sizeof(long));
    long *lp = ::new (buffer) long; // UndCC_Valid

    // ...
    std::free(buffer);
}
}

namespace compliant6
{
struct S
{
    S();
    ~S();
};

void f()
{
    const size_t n = 32;
    alignas(S) unsigned char buffer[sizeof(S) * n + 16];
    S *sp = ::new (buffer) S[n]; // UndCC_Valid

    // ...
    for (size_t i = 0; i != n; ++i)
    {
        sp[i].~S();
    }
}
}

namespace noncompliant1
{
void f()
{
    short s;
    long *lp = ::new (&s) long; // UndCC_Violation
}
}

namespace noncompliant2
{
void f()
{
    char c; // Used elsewhere in the function
    unsigned char buffer[sizeof(long)];
    long *lp = ::new (buffer) long; // UndCC_Violation

    // ...
}
}

namespace noncompliant3
{
struct S
{
    S();
    ~S();
};

void f()
{
    const unsigned N = 32;
    alignas(S) unsigned char buffer[sizeof(S) * N];
    S *sp = ::new (buffer) S[N]; // UndCC_Violation

    // ...
    // Destroy elements of the array.
    for (size_t i = 0; i != N; ++i)
    {
        sp[i].~S();
    }
}
}

namespace noncompliant4
{
void f()
{
    alignas(int) unsigned char buffer[sizeof(long)];
    long *lp = ::new (buffer) long; // UndCC_Violation
}
}

namespace noncompliant5
{
struct Small
{
    int x;
};

struct Big
{
    long a;
    long b;
};

void f()
{
    alignas(Big) unsigned char buffer[sizeof(Small)];
    Big *bp = ::new (buffer) Big; // UndCC_Violation
}
}

namespace noncompliant6
{
void f(void *p)
{
    long *lp = ::new (p) long; // UndCC_Violation
}
}

namespace shadowtest
{
void f()
{
    alignas(long) unsigned char buffer[sizeof(long)];

    {
        unsigned char buffer[sizeof(long)]; // shadows outer buffer, no alignas
        long *lp = ::new (buffer) long; // UndCC_Violation
    }
}
}
