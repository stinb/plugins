// $Id: A10-1-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>
class A
{
public:
    void F1() noexcept(false) {}

private:
    std::int32_t x{0};
    std::int32_t y{0};
};
class B
{
public:
    void F2() noexcept(false) {}

private:
    std::int32_t x{0};
};
class C : public A, // UndCC_Violation - A and B are both not interface classes
          public B
{
};
class D
{
public:
    virtual ~D() = 0;
    virtual void F3() noexcept = 0;
    virtual void F4() noexcept = 0;
};
class E
{
public:
    static constexpr std::int32_t value{10};

    virtual ~E() = 0;
    virtual void F5() noexcept = 0;
};
class F : public A, // UndCC_Violation - A and B are both not interface classes
          public B,
          public D,
          public E
{
};
class G : public A, // UndCC_Valid - D and E are interface classes
          public D,
          public E
{
};
