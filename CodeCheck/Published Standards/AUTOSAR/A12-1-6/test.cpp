#include <cstdint>
#include <string>

class Base
{
  public:
    Base(std::int32_t a, std::int32_t b) : a_(a), b_(b) {}
    explicit Base(std::int32_t a) : Base(a, 0) {}
    Base(const Base& other) : a_(other.a_), b_(other.b_) {}
    Base(Base&& other) : a_(other.a_), b_(other.b_) {}

  private:
    std::int32_t a_;
    std::int32_t b_;
};

// Reimplements both of Base's non-special constructors as pure forwarding
// wrappers (copy/move don't count) - should use an inheriting constructor.
class Derived1 : public Base
{
  public:
    Derived1(std::int32_t a, std::int32_t b) : Base(a, b) {} // UndCC_Violation
    explicit Derived1(std::int32_t a) : Base(a) {} // UndCC_Violation
};

// Already uses an inheriting constructor declaration.
class Derived2 : public Base
{
  public:
    using Base::Base; // UndCC_Valid
};

// Needs further initialization of its own member, so a plain
// inheriting-constructor declaration would leave extra_ uninitialized.
class Derived3 : public Base
{
  public:
    Derived3(std::int32_t a, std::int32_t b) : Base(a, b), extra_(0) {} // UndCC_Valid
    explicit Derived3(std::int32_t a) : Base(a), extra_(0) {} // UndCC_Valid

  private:
    std::int32_t extra_;
};

// Only reimplements one of Base's two constructors - does not "require all
// the constructors from the base class".
class Derived4 : public Base
{
  public:
    explicit Derived4(std::int32_t a) : Base(a) {} // UndCC_Valid
};

// Forwards to Base but swaps the arguments - not a pure passthrough.
class Derived5 : public Base
{
  public:
    Derived5(std::int32_t a, std::int32_t b) : Base(b, a) {} // UndCC_Valid
    explicit Derived5(std::int32_t a) : Base(a) {} // UndCC_Valid
};

// Constructor body does real work beyond forwarding to the base.
class Derived6 : public Base
{
  public:
    Derived6(std::int32_t a, std::int32_t b) : Base(a, b) {} // UndCC_Valid
    explicit Derived6(std::int32_t a) : Base(a)
    {
        log_.append("constructed"); // extra work, not a pure forwarding constructor
    }

  private:
    std::string log_;
};

class Base2
{
  public:
    Base2() = default;
    explicit Base2(std::int32_t a) : a_(a) {}

  private:
    std::int32_t a_{0};
};

// Base2's default constructor is a special member and isn't part of "all
// the constructors" that must be forwarded - only the converting
// constructor is, so this still qualifies as a violation.
class Derived7 : public Base2
{
  public:
    explicit Derived7(std::int32_t a) : Base2(a) {} // UndCC_Violation
};

// A constructor with a default argument is not a pure 1:1 passthrough of
// its own parameter list, so this should not be flagged.
class Derived8 : public Base
{
  public:
    Derived8(std::int32_t a, std::int32_t b = 0) : Base(a, b) {} // UndCC_Valid
    explicit Derived8(std::int32_t a) : Base(a) {} // UndCC_Valid
};

class NoBase
{
  public:
    explicit NoBase(std::int32_t a) {}
};

namespace edge_cases {

namespace ns {
class Base
{
  public:
    Base(std::int32_t a, std::int32_t b) : a_(a), b_(b) {}
    explicit Base(std::int32_t a) : Base(a, 0) {}

  private:
    std::int32_t a_;
    std::int32_t b_;
};
}

// Calls the base constructor with its namespace-qualified name.
class QualifiedBaseName : public ns::Base
{
  public:
    QualifiedBaseName(std::int32_t a, std::int32_t b) : ns::Base(a, b) {} // UndCC_Violation
    explicit QualifiedBaseName(std::int32_t a) : ns::Base(a) {} // UndCC_Violation
};

class PlainBase
{
  public:
    PlainBase(std::int32_t a, std::int32_t b) : a_(a), b_(b) {}
    explicit PlainBase(std::int32_t a) : PlainBase(a, 0) {}

  private:
    std::int32_t a_;
    std::int32_t b_;
};

// An inheriting constructor takes on the accessibility of where the
// using-declaration is written, regardless of the inheritance's own access
// specifier, so private/protected inheritance is still flaggable.
class PrivateDerived : private PlainBase
{
  public:
    PrivateDerived(std::int32_t a, std::int32_t b) : PlainBase(a, b) {} // UndCC_Violation
    explicit PrivateDerived(std::int32_t a) : PlainBase(a) {} // UndCC_Violation
};

class ProtectedDerived : protected PlainBase
{
  public:
    ProtectedDerived(std::int32_t a, std::int32_t b) : PlainBase(a, b) {} // UndCC_Violation
    explicit ProtectedDerived(std::int32_t a) : PlainBase(a) {} // UndCC_Violation
};

class VirtualDerived : public virtual PlainBase
{
  public:
    VirtualDerived(std::int32_t a, std::int32_t b) : PlainBase(a, b) {} // UndCC_Violation
    explicit VirtualDerived(std::int32_t a) : PlainBase(a) {} // UndCC_Violation
};

// Defined out-of-line, away from the class body.
class OutOfLineDerived : public PlainBase
{
  public:
    OutOfLineDerived(std::int32_t a, std::int32_t b);
    explicit OutOfLineDerived(std::int32_t a);
};
OutOfLineDerived::OutOfLineDerived(std::int32_t a, std::int32_t b) : PlainBase(a, b) {} // UndCC_Violation
OutOfLineDerived::OutOfLineDerived(std::int32_t a) : PlainBase(a) {} // UndCC_Violation

// Multiple inheritance is out of scope for this check - not flagged.
class Other
{
  public:
    explicit Other(std::int32_t c) : c_(c) {}

  private:
    std::int32_t c_;
};
class MultiDerived : public PlainBase, public Other
{
  public:
    MultiDerived(std::int32_t a, std::int32_t b) : PlainBase(a, b), Other(0) {} // UndCC_Valid
};

}
