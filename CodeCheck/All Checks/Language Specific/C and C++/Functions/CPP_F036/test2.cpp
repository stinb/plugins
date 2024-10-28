// Custom

class BaseA
{
  virtual void a();
  virtual void b();
  virtual void c();
  virtual void d();
  virtual void e();
  virtual void f();
  ~BaseA();
};

class DerivedA: BaseA
{
  void a();   // UndCC_Violation
  void b() {} // UndCC_Violation
  void c() override;
  void d() override {}
  void e() final;
  void f() final {}
  ~DerivedA();
};

void DerivedA::a() {}
void DerivedA::c() {}
void DerivedA::e() {}


class BaseB
{
  virtual ~BaseB();
};

class DerivedB: BaseB
{
  ~DerivedB(); // UndCC_Violation
};


class BaseC
{
  virtual ~BaseC();
};

class DerivedC: BaseC
{
  ~DerivedC() override;
};
