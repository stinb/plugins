// AUTOSAR

// The lightweight analysis environment has no standard library headers, so
// minimal but complete definitions of std::unique_ptr / std::make_unique are
// provided here. They must be complete (not just declared) so the file has no
// analysis errors and so the constructions resolve to real references.
namespace std {
template <typename T>
class function
{
};

template <typename T, typename D = void>
class unique_ptr
{
public:
  unique_ptr() {}
  explicit unique_ptr(T *p) {}
  template <typename DL> unique_ptr(T *p, DL d) {}
  unique_ptr(unique_ptr &&other) {}
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args... args)
{
  unique_ptr<T> p;
  return p;
}
} // namespace std

struct A
{
  A() {}
  A(unsigned char xx, unsigned char yy) : x(xx), y(yy) {}
  unsigned char x;
  unsigned char y;
};

void Foo(std::unique_ptr<A> a, std::unique_ptr<A> b) {}

int main(void)
{
  // UndCC_Valid
  std::unique_ptr<A> upA = std::make_unique<A>(4, 6);
  // non-compliant
  std::unique_ptr<A> upA2 = std::unique_ptr<A>(new A(5, 7)); // UndCC_Violation

  // non-compliant, potential memory leak, as A class constructor throws
  Foo(std::unique_ptr<A>(new A()), std::unique_ptr<A>(new A())); // UndCC_Violation

  // non-compliant, potential memory leak, as A class constructor throws
  Foo(std::make_unique<A>(4, 6), std::unique_ptr<A>(new A())); // UndCC_Violation

  // UndCC_Valid, no memory leaks
  Foo(std::make_unique<A>(4, 6), std::make_unique<A>(4, 6));

  // UndCC_Valid by exception (custom deleter)
  std::unique_ptr<A, std::function<void(A *)>> ptr(new A(4, 5),
                                                   [](A *b) { delete b; });

  return 0;
}
