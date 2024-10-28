// Custom

class A
{
  public:
    virtual void V() noexcept = 0;
};

class B : public A
{
  public:
    void V() noexcept // UndCC_Violation
    {
    }
};

class C : public A
{
  public:
    void V() noexcept final // UndCC_Valid
    {
    }
};
