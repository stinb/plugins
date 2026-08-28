// MISRA C++ 2008 - 5-2-2

// The standard's own non-compliant line, static_cast< D * >( pB ), cannot be
// tested: a static_cast or C-style cast from a virtual base is a compile
// error, so the cast has to be written another way to exist at all.

class B { public: virtual ~B(); int b; };
class D : public virtual B { public: int d; };
class VG : public D { public: int vg; };   // B is an indirect virtual base
class ND : public B { public: int nd; };   // B is not a virtual base
class U { public: int u; };

void f(B *pB, D *pD, U *pU)
{
  D *a = dynamic_cast<D *>(pB);                     // UndCC_Valid
  D &b = dynamic_cast<D &>(*pB);                    // UndCC_Valid
  D *c = reinterpret_cast<D *>(pB);                 // UndCC_Violation
  D &e = reinterpret_cast<D &>(*pB);                // UndCC_Violation
  D *g = (D *)(void *)pB;                           // UndCC_Violation - laundered through void *
  D *h = static_cast<D *>(static_cast<void *>(pB)); // UndCC_Violation
  VG *i = reinterpret_cast<VG *>(pB);               // UndCC_Violation - indirect virtual base
  ND *j = reinterpret_cast<ND *>(pB);               // UndCC_Valid - base is not virtual
  B *k = static_cast<B *>(pD);                      // UndCC_Valid - upcast
  U *l = reinterpret_cast<U *>(pB);                 // UndCC_Valid - unrelated type
  D *m = static_cast<D *>(static_cast<void *>(pU)); // UndCC_Valid - unrelated type

  (void)a; (void)b; (void)c; (void)e; (void)g; (void)h;
  (void)i; (void)j; (void)k; (void)l; (void)m;
}

class L : public virtual B { public: int l; };
class R : public virtual B { public: int r; };
class Dia : public L, public R { public: int dia; };  // two virtual paths to B
typedef D *DPtr;

struct S { D *m; S(B *p); };
S::S(B *p) : m(reinterpret_cast<D *>(p)) {}   // UndCC_Violation - member initializer

#define CAST(T, e) reinterpret_cast<T>(e)

template <typename T>
D *make(T *p) { return reinterpret_cast<D *>(p); }  // UndCC_Violation - instantiated with B *

void g(B *pB, const B *pcB, B **ppB)
{
  ND *a = static_cast<ND *>(pB);                       // UndCC_Valid - legal downcast, B is not a virtual base of ND
  const D *b = reinterpret_cast<const D *>(pcB);       // UndCC_Violation
  D *c = reinterpret_cast<D *>(const_cast<B *>(pcB));  // UndCC_Violation - const_cast in the chain
  DPtr d = reinterpret_cast<DPtr>(pB);                 // UndCC_Violation - typedef destination
  Dia *e = reinterpret_cast<Dia *>(pB);                // UndCC_Violation - two virtual paths to B
  D *h = CAST(D *, pB);                                // UndCC_Violation - cast in a macro
  D *i = make(pB);                                     // UndCC_Valid - reported on the template
  D **j = reinterpret_cast<D **>(ppB);                 // UndCC_Valid - not a pointer to a virtual base
  void *v = pB;
  D *k = static_cast<D *>(v);                          // UndCC_FalseNeg - provenance lost through the variable

  (void)a; (void)b; (void)c; (void)d; (void)e;
  (void)h; (void)i; (void)j; (void)k;
}

typedef class { public: virtual void v(); int a; } Anon;   // class named only by the typedef
class DA : public virtual Anon { public: int da; };

void n(Anon *pA)
{
  DA *a = reinterpret_cast<DA *>(pA);   // UndCC_Violation
  (void)a;
}
