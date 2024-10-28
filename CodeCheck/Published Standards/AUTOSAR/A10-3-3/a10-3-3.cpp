// $Id: A10-3-3.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
class A
{
public:
    virtual ~A() = default;
    virtual void F() noexcept = 0;
    virtual void G() noexcept {}
};
class B final : public A
{
public:
    void F() noexcept final // UndCC_Valid
    {
    }
    void G() noexcept override // UndCC_Violation
    {
    }
    virtual void H() noexcept = 0; // UndCC_Violation
    virtual void Z() noexcept      // UndCC_Violation
    {
    }
};
