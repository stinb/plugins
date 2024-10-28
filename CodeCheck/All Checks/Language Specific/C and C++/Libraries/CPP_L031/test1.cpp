#include <typeinfo>
bool SideEffects() noexcept
{
    return true;
}
class A
{
  public:
    static A& F1() noexcept {
        return a;
    }
    virtual ~A() {}

  private:
    static A a;
};
A A::a;
int main()
{
    bool (*ptrSideEffects)() = &SideEffects;
    static A& (*ptrF1)() = &A::F1;

    typeid(ptrSideEffects()); // UNDCC_Violation - sideEffects function not called
    typeid(ptrF1());          // UNDCC_Violation - A::f1() function called to determine the polymorphic type

    return 0;
}
