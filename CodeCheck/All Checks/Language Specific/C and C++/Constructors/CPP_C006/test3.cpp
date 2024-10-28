// Custom: template

template<typename T>
class A
{
  public:
    A(T x)
    {

    }
};

template<typename T>
class B1 : A<T>
{
  public:
    B1(T x): A<T>(x)
    {

    }
};

template<typename T>
class B2 : A<T>
{
  public:
    B2(T x) // UndCC_Violation
    {

    }
};
