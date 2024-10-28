#include <string>
#include <locale>

void capitalize(std::string &s)
{
    std::locale loc;
    s.front() = std::use_facet<std::ctype<char>>(loc).toupper(s.front()); // UndCC_Violation, two violations for using front function on s
}
