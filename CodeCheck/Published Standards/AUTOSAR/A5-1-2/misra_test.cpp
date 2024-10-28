#include <vector>
#include <cstddef>
#include <functional>
#include <iostream>

void bar(double val, double min, double max)
{
    auto const easedVal = [&]() // UndCC_Valid
    {
        if (val < min)
        {
            return (val + min) / 2;
        }
        if (val > max)
        {
            return (val + max) / 2;
        }
        return val;
    }();
    auto const ease = [&]() // UndCC_Violation
    {
        if (val < min)
        {
            return (val + min) / 2;
        }
        if (val > max)
        {
            return (val + max) / 2;
        }
        return val;
    };
    ease(); // - not an immediate call
}
template <typename It, typename Func>
bool f1(It b, It e, Func f) // f1 does not store f
{
    for (It it = b; it != e; ++it)
    {
        if (f(*it)) // f is called
        {
            return true;
        }
    }
    return false;
}
template <typename Cont, typename Func>
bool f2(Cont const &c, Func f) // f2 does not store f
{
    return f1(std::begin(c), std::end(c), f); // f passed to non-storing function
}
void foo(std::vector<size_t> const &v, size_t i)
{
    bool b1 = f1(v.cbegin(), v.cend(),
                 [&](size_t elem) // UndCC_FalsePos, limitation
                 { return elem == i; });
    bool b2 = f2(v,
                 [&](size_t elem) // UndCC_FalsePos, limitation
                 { return elem == i; });
}
struct Speedometer
{
    std::vector<std::function<void(double)>> observers;
    template <typename Func>
    void addObserver(Func f) // addObserver stores f
    {
        observers.push_back(f); // Copying f to the std::function
    }
};
void process(std::function<Speedometer()>);
auto f3()
{
    Speedometer s;
    process([&]() // UndCC_Violation - conversion to std::function stores the lambda
            { return s; });
    return [=]() // UndCC_Violation - implicit capture of s
    { return s; };
}
void addLoggers(Speedometer s, std::ostream &os)
{
    s.addObserver([&](double speed) // UndCC_Violation - implicit capture
                  { os << speed; });
    s.addObserver([&os](double speed) // UndCC_Valid - explicit capture
                  { os << speed; });
    s.addObserver([](double speed) // UndCC_Valid - no capture
                  { std::cout << speed; });
}