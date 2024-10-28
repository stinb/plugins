#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>

class S
{
    int i;

public:
    S() : i(0) {}
    S(int i) : i(i) {}
    S(const S &) = default;
    S &operator=(const S &) = default;
};

template <typename Iter>
void f(Iter i, Iter e)
{
    static_assert(std::is_same<typename std::iterator_traits<Iter>::value_type, S>::value,
                  "Expecting iterators over type S");
    ptrdiff_t count = std::distance(i, e);
    if (!count)
    {
        return;
    }

    // Get some temporary memory.
    auto p = std::get_temporary_buffer<S>(count);
    if (p.second < count)
    {
        // Handle error; memory wasn't allocated, or insufficient memory was allocated.
        return;
    }
    S *vals = p.first;

    // Copy the values into the memory.
    std::copy(i, e, vals); // UndCC_FalseNeg

    // ...

    // Return the temporary memory.
    std::return_temporary_buffer(vals);
}
