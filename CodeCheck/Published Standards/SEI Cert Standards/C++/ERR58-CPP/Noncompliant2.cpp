#include <string>

static const std::string global("..."); // UndCC_Violation

int main()
try 
{
    // ...
}
catch (...)
{
    // IMPORTANT: Will not catch exceptions thrown
    // from the constructor of global
}
