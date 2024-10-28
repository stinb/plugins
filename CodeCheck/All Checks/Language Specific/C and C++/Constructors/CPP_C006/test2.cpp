// Custom: namespace

namespace API {

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

class C1 : public B
{
  public:
    C1 ( ) // UndCC_Violation - benign, but should be C1 ( ) : B ( )
    {
    }
};

class C2 : public B
{
  public:
    C2 ( ) : B ( )
    {
    }
};

}
