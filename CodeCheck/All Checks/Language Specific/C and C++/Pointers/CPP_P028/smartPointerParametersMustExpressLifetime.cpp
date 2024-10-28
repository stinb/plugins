// $Id: A8-4-11.cpp 307966 2018-02-16 16:03:46Z christof.meerwald $

#include <cstdint>
#include <memory>
#include <numeric>
#include <vector>

class A
{
    public:
    void do_stuff();
};

void foo(std::shared_ptr<A> a) // UndCC_Violation - passing object as smart pointer
{
    if (a)
    {
        a->do_stuff();
    }
    else
    {
        // ...
    }
}

void bar(A *a) // UndCC_Valid - passing as raw pointer instead
{
    if (a != nullptr)
    {
        a->do_stuff();
    }
    else
    {
        // ...
    }
}


class B
{
public:
    void add_a(std::shared_ptr<A> a)
    {
        m_v.push_back(a);
    }

private:
    std::vector<std::shared_ptr<A>> m_v;
};

void bar(B &b, std::shared_ptr<A> a) // UndCC_Valid - storing the shared pointer (affecting lifetime)
{
    b.add_a(a);
}
