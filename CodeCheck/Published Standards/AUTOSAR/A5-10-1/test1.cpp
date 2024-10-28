// Custom

class A
{
  public:
    virtual ~A() = default;
    void F1() noexcept {}
    void F2() noexcept {}
    virtual void F3() noexcept {}
};

void Fn()
{
  bool b1 = (&A::F2 == &A::F1);   // UndCC_Valid
  bool b2 = (nullptr == &A::F1);  // UndCC_Valid
  bool b3 = (nullptr == &A::F3);  // UndCC_Valid
  bool b4 = (nullptr != &A::F3);  // UndCC_Valid
  bool b5 = (&A::F1 == &A::F3);   // UndCC_Violation
}
