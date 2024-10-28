// $Id: A20-8-7.cpp 308795 2018-02-23 09:27:03Z michal.szczepankiewicz $

#include <memory>

template <template <typename> class T, typename U>
struct Base
{
    T<U> sp;
};

template <typename T>
using Shared = Base<std::shared_ptr, T>;

template <typename T>
using Weak = Base<std::weak_ptr, T>;

struct struct1;

struct struct2 : public Shared<struct1>
{
};

struct struct1 : public Shared<struct2>
{
};

struct A : public Shared<A>
{
};

struct struct3;

struct struct4 : public Shared<struct3>
{
};

struct struct3 : public Weak<struct4>
{
};

int main()
{
    std::shared_ptr<struct2> obj_1 = std::make_shared<struct2>();
    std::shared_ptr<struct1> obj_2 = std::make_shared<struct1>();
    obj_1->sp = obj_2; // UndCC_Violation
    obj_2->sp = obj_1; // UndCC_Violation
    //non-compliant, both obj_1 and obj_2 have ref_count() == 2
    //destructors of obj_1 and obj_2 reduce ref_count() to 1,
    //destructors of underlying objects are never called,
    //so destructors of shared_ptrs sp are not called
    //and memory is leaked

    std::shared_ptr<A> a = std::make_shared<A>();
    a->sp = a;  // UndCC_Violation, object ’a’ destructor does not call underlying memory destructor

    std::shared_ptr<struct4> obj_1b = std::make_shared<struct4>();
    std::shared_ptr<struct3> obj_2b = std::make_shared<struct3>();
    obj_1b->sp = obj_2b; /* UNDCC_Valid */
    obj_2b->sp = obj_1b; /* UNDCC_Valid */

    return 0;
}
