// $Id: A8-4-6.cpp 305588 2018-01-29 11:07:35Z michal.szczepankiewicz $

#include <string>
#include <vector>

class A
{
public:
    explicit A(std::vector<std::string> &&v);
};

class B
{
public:
    explicit B(const std::vector<std::string> &v);
};

template <typename T, typename... Args>
T make(Args &&...args)
{
    return T{std::forward<Args>(args)...}; // UndCC_Valid, forwarding args
}

auto glambda = [](auto &&a) // UndCC_Violation
{ return a; };

auto glambda_2 = [](auto &&a) // UndCC_Valid
{ return std::forward(a); };

auto glambda_3 = [](auto &&a) // UndCC_Violation
{
    a = 1;
    return std::forward(a);
};

int main()
{
    make<A>(std::vector<std::string>{});

    std::vector<std::string> v;
    make<B>(v);
}
