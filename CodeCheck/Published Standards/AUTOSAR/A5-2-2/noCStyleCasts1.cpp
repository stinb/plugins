// $Id: A5-2-2.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
class C
{
  public:
    explicit C(std::int32_t) {}
    virtual void Fn() noexcept {}
};
class D : public C
{
  public:
    void Fn() noexcept override {}
};
class E
{
};
std::int32_t G() noexcept
{
    return 7;
}
void F() noexcept(false)
{
    C a1 = C{10};       // UNDCC_Valid
    C* a2 = (C*)(&a1);  // UNDCC_Violation
    const C a3(5);
    C* a4 = const_cast<C*>(&a3);       // UNDCC_Valid - violates another rule
    E* d1 = reinterpret_cast<E*>(a4);  // UNDCC_Valid - violates another rule
    D* d2 = dynamic_cast<D*>(a2);      // UNDCC_Valid - violates another rule
    std::int16_t x1 = 20;
    std::int32_t x2 = static_cast<std::int32_t>(x1);  // UNDCC_Valid
    std::int32_t x3 = (std::int32_t)x1;               // UNDCC_Violation
    std::int32_t x4 = 10;
    float f1 = static_cast<float>(x4);                // UNDCC_Valid
    float f2 = (float)x4;                             // UNDCC_Violation
    std::int32_t x5 = static_cast<std::int32_t>(f1);  // UNDCC_Valid
    std::int32_t x6 = (std::int32_t)f1;               // UNDCC_Violation
    (void)G();                                        // UNDCC_Violation
    static_cast<void>(G());                           // UNDCC_Valid
}
