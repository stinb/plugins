// AUTOSAR C++ 2014

class A
{
  public:
    void F2() {}
};
void fn1(A* a<:10:>) // UndCC_Violation
<%                   // UndCC_Violation
  a<:0:>->F2();      // UndCC_Violation
%>                   // UndCC_Violation
void Fn2(A* a[10])   // UndCC_Valid, equivalent to the above
{
  a[0]->F2();
}
