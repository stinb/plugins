#include <string>
#include <locale>

void capitalize(std::string &s)
{
    if (s.empty())
    {
        return;
    }

    std::locale loc;
    s.front() = std::use_facet<std::ctype<char>>(loc).toupper(s.front());
}