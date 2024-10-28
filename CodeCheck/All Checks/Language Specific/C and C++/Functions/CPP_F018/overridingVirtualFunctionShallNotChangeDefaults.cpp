#include <cstdint>

int32_t x();

class Base
{
public:
    virtual void g1(int32_t a = 0);
    virtual void g2(int32_t a = 0);
    virtual void b1(int32_t a = 0);
    virtual void f1(int32_t a = 1, int32_t b = 2);
    virtual void f2(int32_t a);
    virtual void f3(int32_t a = 1, int32_t b = 2);
    virtual void f4(int32_t);
    virtual void f5(int32_t);
    virtual void f6(int32_t);
    virtual void f7(int32_t a = x());
};
class Derived : public Base
{
public:
    virtual void g1(int32_t a = 0);                // UNDCC_Valid - same default used
    virtual void g2(int32_t a);                    // UNDCC_Valid - no default specified
    virtual void b1(int32_t a = 10);               // UNDCC_Violation - different value
    virtual void f1(int32_t a = 1, int32_t b = 0); // UNDCC_Violation - different value for b
    virtual void f2(int32_t a = 1);                // UNDCC_Violation - different value
    virtual void f3(int32_t a, int32_t b = 2);     // UNDCC_Valid  - no default specified for a, same for b
    virtual void f4(int32_t a = 0);                // UNDCC_Violation - different value
    virtual void f5(int32_t a);                    // UNDCC_Valid - no default specified
    virtual void f6(int32_t);                      // UNDCC_Valid - no default specified
    void f7(int32_t a = x()) override;             // UNDCC_Violation - not constant, MISRA23_13.3.2
};
void f(Derived &d)
{
    Base &b = d;
    b.g1();  // Will use default of 0
    d.g1();  // Will use default of 0
    b.g2();  // Will use default of 0
    d.g2(0); // No default value available to use
    b.b1();  // Will use default of 0
    d.b1();  // Will use default of 10
}
