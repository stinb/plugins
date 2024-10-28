#include <stdexcept>
#include <type_traits>

class S : public std::exception
{
    std::runtime_error m;

public:
    S(const char *msg) : m(msg) {}

    const char *what() const noexcept override
    {
        return m.what();
    }
};

static_assert(std::is_nothrow_copy_constructible<S>::value,
              "S must be nothrow copy constructible");

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
    catch (S &s)
    {
        // Handle error
    }
}