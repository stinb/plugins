#include <exception>
#include <string>

class S : public std::exception
{
    std::string m;

public:
    S(const char *msg) : m(msg) {}

    const char *what() const noexcept override
    {
        return m.c_str();
    }
};

void g()
{
    // If some condition doesn't hold...
    throw S("Condition did not hold");
}

void f()
{
    try
    {
        g();
    }
    catch (S &s) // UndCC_Violation
    {
        // Handle error
    }
}