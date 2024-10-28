#include <string>

extern std::size_t get_index();

void f()
{
    std::string s("01234567");
    std::size_t i = get_index();
    if (i < s.length())
    {
        s[i] = '1';
    }
    else
    {
        // Handle error
    }
}