// $Id: A8-4-5.cpp 305588 2018-01-29 11:07:35Z michal.szczepankiewicz $

#include <string>
#include <vector>

class A
{
public:
    explicit A(std::vector<std::string> &&v) // UndCC_Valid, move from consume parameter
        : m_v{std::move(v)}
    {
    }

private:
    std::vector<std::string> m_v;
};

class B
{
public:
    explicit B(std::vector<std::string> &&v) // UndCC_Violation, consume parameter not moved from
        : m_v{v}
    {
    }

    std::vector<std::string> m_v;
};
