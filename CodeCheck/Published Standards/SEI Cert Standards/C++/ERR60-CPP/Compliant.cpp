#include <stdexcept>
#include <type_traits>

struct S : std::runtime_error
{
    S(const char *msg) : std::runtime_error(msg) {}
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
