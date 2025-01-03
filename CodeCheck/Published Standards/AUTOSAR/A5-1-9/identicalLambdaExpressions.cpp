// $Id: A5-1-9.cpp 307019 2018-02-09 15:16:47Z christof.meerwald $
#include <algorithm>
#include <cstdint>
#include <vector>

void Fn1(const std::vector<int16_t> &v)
{
    if (std::none_of(v.begin(), v.end(),
            [] (int16_t i) { return i < 0; }))
    {
        // ...
    }
    else if (std::all_of(v.begin(), v.end(),
            [] (int16_t i) { return i < 0; })) // UndCC_Violation - identical unnamed lambda expression
    {
        // ...
    }
}

void Fn2(const std::vector<int16_t> &v)
{
    auto is_negative = [] (int16_t i) { return i < 0; }; // UndCC_Valid - re-using lambda expression

    if (std::none_of(v.begin(), v.end(), is_negative))
    {
        // ...
    }
    else if (std::all_of(v.begin(), v.end(), is_negative))
    {
        // ...
    }
}
