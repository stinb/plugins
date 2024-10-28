#include <vector>
#include <string>

extern std::vector<std::string> make();

void f()
{
    for (char c : make().at(0)) // UndCC_Violation - two function calls
    {
    }
}

void g()
{
    auto range = make().at(0); // Note that auto && would dangle
    for (char c : range)       // UndCC_Valid - no call when using named range
    {
    }
}

void h()
{
    for (auto s : make()) // UndCC_Valid - single function call
    {
    }
}
