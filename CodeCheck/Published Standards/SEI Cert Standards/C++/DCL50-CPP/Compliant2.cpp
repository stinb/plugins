// SEI CERT C++

#include <type_traits>

template <typename Arg, typename... Ts, typename std::enable_if<std::is_integral<Arg>::value>::type * = nullptr>
int add(Arg i, Arg j, Ts... all)
{
    int values[] = {j, all...};
    int r = i;
    for (auto v : values)
    {
        r += v;
    }
    return r;
}
