// MISRA C++ 2008

#include <cstdint>

class A
{
  public:
    A ( )
    {
    }
};

class B : public A
{
  public:
    B ( ) // UndCC_Violation - benign, but should be B ( ) : A ( )
    {
    }
};

class V
{
  public:
    V ( )
    {
    }
    V ( int32_t i )
    {
    }
};

class C1 : public virtual V
{
  public:
    C1 ( ) : V ( 21 )
    {
    }
};

class C2 : public virtual V
{
  public:
    C2 ( ) : V ( 42 )
    {
    }
};

class D: public C1, public C2
{
  public:
    D ( ) // UndCC_Violation
    {
    }
};
