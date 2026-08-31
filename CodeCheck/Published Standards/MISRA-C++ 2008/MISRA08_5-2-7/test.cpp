// MISRA C++ 2008 - 5-2-7

#include <cstdint>

struct S
{
  std::int32_t i;
  std::int32_t j;
};

class C
{
public:
  std::int32_t i;
  std::int32_t j;
  virtual ~C();
};

class Derived : public C { public: std::int32_t d; };

void f(S *s)
{
  C *c = reinterpret_cast<C *>(s);                    // UndCC_Violation
  std::intptr_t i = reinterpret_cast<std::intptr_t>(s); // UndCC_Valid - breaks 5-2-9, not this rule
  C *d = reinterpret_cast<C *>(i);                    // UndCC_FalseNeg - provenance lost through the variable
  S *e = reinterpret_cast<S *>(i);                    // UndCC_Valid - breaks 5-2-8, not this rule

  (void)c; (void)d; (void)e;
}

void g(S *s, C *pc, Derived *pd, std::int32_t *pi, const S *pcs, S arr[4])
{
  C *a = reinterpret_cast<C *>(static_cast<void *>(s));  // UndCC_Violation - laundered through void *
  C *b = (C *)(std::intptr_t)s;                          // UndCC_Violation - laundered through an integer
  C *c = reinterpret_cast<C *>(arr);                     // UndCC_Violation - array decays to S *
  float *d = reinterpret_cast<float *>(pi);              // UndCC_Violation - unrelated scalar types

  C *e = static_cast<C *>(pd);                           // UndCC_Valid - derived to base
  Derived *g2 = static_cast<Derived *>(pc);              // UndCC_Valid - base to derived
  const S *h = const_cast<const S *>(s);                 // UndCC_Valid - same type
  S *i = const_cast<S *>(pcs);                           // UndCC_Valid - same type, qualification only
  void *j = static_cast<void *>(s);                      // UndCC_Valid - void is 5-2-8's concern
  S *k = static_cast<S *>(j);                            // UndCC_Valid - from void, 5-2-8's concern
  std::intptr_t l = reinterpret_cast<std::intptr_t>(s);  // UndCC_Valid - not a pointer destination

  (void)a; (void)b; (void)c; (void)d; (void)e; (void)g2;
  (void)h; (void)i; (void)j; (void)k; (void)l;
}

class PA { public: virtual ~PA(); };
class PB { public: virtual ~PB(); };
class Incomplete;

typedef S *SPtr;
typedef void (*FnPtr)(int);

#define CAST_TO_C(e) reinterpret_cast<C *>(e)

template <typename T>
C *make(T *p) { return reinterpret_cast<C *>(p); }  // UndCC_Violation - instantiated with S *

void h(S *s, S **ss, PA *pa, SPtr sp, FnPtr fp, void (*fp2)(int))
{
  PB *a = dynamic_cast<PB *>(pa);        // UndCC_Valid - a cross-cast is specified
  C &b = reinterpret_cast<C &>(*s);      // UndCC_Valid - the rule is about pointer types
  C *c = CAST_TO_C(s);                   // UndCC_Violation - cast in a macro
  C *d = make(s);                        // UndCC_Valid - reported on the template
  C *e = reinterpret_cast<C *>(sp);      // UndCC_Violation - typedef'd source
  C **g = reinterpret_cast<C **>(ss);    // UndCC_Violation - multi-level pointer
  FnPtr i = reinterpret_cast<FnPtr>(fp2);      // UndCC_Valid - function pointers are 5-2-6's concern
  C *j = reinterpret_cast<C *>(fp);            // UndCC_Valid - function pointer source
  Incomplete *k = reinterpret_cast<Incomplete *>(s);  // UndCC_Violation - incomplete destination

  (void)a; (void)b; (void)c; (void)d; (void)e;
  (void)g; (void)i; (void)j; (void)k;
}
