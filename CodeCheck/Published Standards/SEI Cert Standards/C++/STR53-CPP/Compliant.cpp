#include <stdexcept>
#include <string>
extern std::size_t get_index();

void f()
{
    std::string s("01234567");
    try
    {
        s.at(get_index()) = '1';
    }
    catch (std::out_of_range &)
    {
        // Handle error
    }
}