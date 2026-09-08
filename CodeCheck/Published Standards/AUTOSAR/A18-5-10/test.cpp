// AUTOSAR A18-5-10

#include <new>
#include <cstdint>

void Foo()
{
  std::uint8_t c;
  std::uint64_t *ptr = ::new (&c) std::uint64_t;   // UndCC_Violation - insufficient storage
  (void)ptr;
}

void Bar()
{
  std::uint8_t buf[sizeof(std::uint64_t)];
  std::uint64_t *ptr = ::new (buf) std::uint64_t;  // UndCC_Violation - storage not properly aligned
  (void)ptr;
}

void Good()
{
  alignas(std::uint64_t) std::uint8_t buf[sizeof(std::uint64_t)];
  std::uint64_t *ptr = ::new (buf) std::uint64_t;  // UndCC_Valid - aligned and large enough
  (void)ptr;
}

struct Big { std::uint64_t a; std::uint64_t b; };

void Arrays()
{
  alignas(Big) std::uint8_t small[sizeof(Big)];
  Big *p = ::new (small) Big[2];                   // UndCC_Violation - two objects do not fit
  alignas(Big) std::uint8_t big[sizeof(Big) * 2];
  Big *q = ::new (big) Big[2];                     // UndCC_Valid
  (void)p; (void)q;
}

void Unknown(void *raw, std::uint8_t *bytes)
{
  std::uint64_t *a = ::new (raw) std::uint64_t;    // UndCC_Valid - storage is not known
  std::uint64_t *b = ::new (bytes) std::uint64_t;  // UndCC_Valid - pointer parameter
  (void)a; (void)b;
}

void Plain()
{
  std::uint64_t *heap = new std::uint64_t;         // UndCC_Valid - not placement new
  std::uint64_t storage;
  std::uint64_t *exact = ::new (&storage) std::uint64_t;  // UndCC_Valid - exact fit
  delete heap;
  (void)exact;
}

#include <type_traits>

// The idiomatic storage for placement new
struct Holder
{
  std::aligned_storage<sizeof(Big), alignof(Big)>::type storage;
  std::uint8_t tiny[1];

  void make()
  {
    Big *p = ::new (&storage) Big;   // UndCC_Valid - aligned_storage is sized and aligned
    Big *q = ::new (tiny) Big;       // UndCC_FalseNeg - a member is not resolved to its storage
    (void)p; (void)q;
  }
};

union U
{
  std::uint64_t u;
  std::uint8_t bytes[sizeof(std::uint64_t)];
};

void UnionStorage()
{
  U u;
  std::uint64_t *p = ::new (&u) std::uint64_t;   // UndCC_Valid - the union is aligned to 8
  (void)p;
}

void ExactStruct()
{
  Big b;
  Big *p = ::new (&b) Big;                        // UndCC_Valid - same type
  (void)p;
}

void SmallerType()
{
  alignas(Big) std::uint8_t buf[sizeof(Big)];
  std::uint32_t *p = ::new (buf) std::uint32_t;   // UndCC_Valid - smaller type in larger storage
  (void)p;
}

#include <cstddef>

struct Arena
{
  std::uint8_t pool[4096];
};

void *operator new(std::size_t, Arena &a) noexcept { return a.pool; }

struct WithMember
{
  static void *operator new(std::size_t, void *p) noexcept { return p; }
};

void NotStorage(Arena &arena)
{
  std::uint64_t *a = new (std::nothrow) std::uint64_t;  // UndCC_Valid - nothrow is a tag, not storage
  Big *b = new (std::nothrow) Big;                      // UndCC_Valid - nothrow is a tag, not storage
  Big *c = new (arena) Big;                             // UndCC_Valid - an allocator, not storage
  (void)a; (void)b; (void)c;
  delete a; delete b;
}

void ReferenceStorage()
{
  alignas(std::uint64_t) std::uint8_t buf[sizeof(std::uint64_t)];
  std::uint8_t &r = buf[0];
  std::uint64_t *p = ::new (&r) std::uint64_t;   // UndCC_Valid - a reference says nothing about the storage
  std::uint64_t *q = ::new (&buf[0]) std::uint64_t;  // UndCC_Valid - not resolved to the array
  std::uint64_t *s = ::new (buf + 0) std::uint64_t;  // UndCC_Valid - not resolved to the array
  (void)p; (void)q; (void)s;
}

void MemberOperator()
{
  alignas(WithMember) std::uint8_t buf[sizeof(WithMember)];
  WithMember *p = ::new (buf) WithMember;   // UndCC_Valid - fits and is aligned
  (void)p;
}
