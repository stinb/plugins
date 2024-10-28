// Custom: typedef

class A
{
  public:
    A()
    {

    }
    A(int x)
    {

    }
};

class B1 : A
{
  private:
    typedef A Base;
  public:
    B1(int x): Base(x)
    {

    }
};

class B2 : A
{
  private:
    typedef A Base;
  public:
    B2(int x) // UndCC_Violation
    {

    }
};
