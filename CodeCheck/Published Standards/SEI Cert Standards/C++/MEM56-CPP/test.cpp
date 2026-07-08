#include <memory>

namespace compliant1
{
void f()
{
    std::shared_ptr<int> p1 = std::make_shared<int>();
    std::shared_ptr<int> p2(p1); // UndCC_Valid
}
}

namespace compliant2
{
struct B
{
    virtual ~B() = default; // Polymorphic object
                            // ...
};
struct D : B
{
};

void g(std::shared_ptr<D> derived);

void f()
{
    std::shared_ptr<B> poly(new D);
    // ...
    g(std::dynamic_pointer_cast<D, B>(poly)); // UndCC_Valid
    // poly is still referring to a valid pointer value.
}
}

namespace compliant3
{
struct S : std::enable_shared_from_this<S>
{
    std::shared_ptr<S> g() { return shared_from_this(); } // UndCC_Valid
};

void f()
{
    std::shared_ptr<S> s1 = std::make_shared<S>();
    std::shared_ptr<S> s2 = s1->g();
}
}

namespace compliant4
{
void f()
{
    std::shared_ptr<int> p1 = std::make_shared<int>();
    std::shared_ptr<int> p2(p1);
    std::shared_ptr<int> p3(p2); // UndCC_Valid
}
}

namespace compliant5
{
void f()
{
    int *i = new int;
    std::shared_ptr<int> p1(i); // UndCC_Valid
    // ...
}

void g()
{
    int *i = new int;
    std::shared_ptr<int> p1(i); // UndCC_Valid
    // ...
}
}

namespace compliant6
{
void f()
{
    int *i = new int;
    std::unique_ptr<int> p1(i);
    std::unique_ptr<int> p2(std::move(p1)); // UndCC_Valid
}
}

namespace noncompliant1
{
void f()
{
    int *i = new int;
    std::shared_ptr<int> p1(i);
    std::shared_ptr<int> p2(i); // UndCC_Violation
}
}

namespace noncompliant2
{
struct B {
  virtual ~B() = default; // Polymorphic object
  // ...
};
struct D : B {};

void g(std::shared_ptr<D> derived);

void f() {
  std::shared_ptr<B> poly(new D);
  // ...
  g(std::shared_ptr<D>(dynamic_cast<D *>(poly.get()))); // UndCC_Violation
  // Any use of poly will now result in accessing freed memory.
}
}

namespace noncompliant3
{
struct S
{
    std::shared_ptr<S> g() { return std::shared_ptr<S>(this); } // UndCC_Violation
};

void f()
{
    std::shared_ptr<S> s1 = std::make_shared<S>();
    // ...
    std::shared_ptr<S> s2 = s1->g();
}
}

namespace noncompliant4
{
void f()
{
    int *i = new int;
    std::unique_ptr<int> p1(i);
    std::unique_ptr<int> p2(i); // UndCC_Violation
}
}

namespace noncompliant5
{
struct B
{
    virtual ~B() = default;
};

void f() {
    std::shared_ptr<B> poly = std::make_shared<B>();
    // ...
    std::shared_ptr<B> other(poly.get()); // UndCC_Violation
    // Any use of poly will now result in accessing freed memory.
}
}

namespace noncompliant6
{
void f()
{
    int *i = new int;
    std::shared_ptr<int> p1(i);
    std::shared_ptr<int> p2;
    // ...
    p2.reset(i); // UndCC_Violation
    // p1 and p2 now both believe they own i.
}
}

namespace noncompliant7
{
struct B
{
    virtual ~B() = default;
};
struct D : B
{
};

void f() {
    std::shared_ptr<B> poly(new D);
    // ...
    std::shared_ptr<D> derived(dynamic_cast<D *>(poly.get())); // UndCC_Violation
    // Any use of poly will now result in accessing freed memory.
}
}

namespace noncompliant8
{
struct S
{
    std::shared_ptr<S> self;

    void link()
    {
        self = std::shared_ptr<S>(this); // UndCC_Violation
    }
};
}
