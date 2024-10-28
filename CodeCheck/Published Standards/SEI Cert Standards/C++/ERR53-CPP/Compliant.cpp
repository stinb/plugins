#include <string>

class C
{
    std::string str;

public:
    C(const std::string &s)
    try : str(s)
    {
        // ...
    }
    catch (...)
    {
        if (!s.empty())
        {
            // ...
        }
    }
};