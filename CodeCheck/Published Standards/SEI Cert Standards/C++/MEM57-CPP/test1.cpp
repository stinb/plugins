// SEI CERT C++

#include <cstdlib>
#include <new>

#ifdef _WIN32
  #define aligned_alloc _aligned_malloc
  #define aligned_free _aligned_free
#else
  #define aligned_alloc std::aligned_alloc
  #define aligned_free free
#endif

struct alignas(32) Vector {
  char elems[32];
  static void *operator new(size_t nbytes) {
    if (void *p = aligned_alloc(alignof(Vector), nbytes)) {
      return p;
    }
    throw std::bad_alloc();
  }
  static void operator delete(void *p) {
    aligned_free(p);
  }
};

Vector *f() {
  Vector *pv = new Vector; // UndCC_Valid
  return pv;
}
