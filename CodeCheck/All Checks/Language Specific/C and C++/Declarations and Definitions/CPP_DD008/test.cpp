template <class T1>
class A
{
public:
  template <class T2>
  class B;

  T1    _a;
};

template <class T1>
template <class T2>
class A<T1>::B // UndCC_Violation
{
public:
  T2 _b;
};
