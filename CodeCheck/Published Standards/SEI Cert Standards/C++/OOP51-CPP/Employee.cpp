#include "Employee.h"

Employee::Employee(const std::string &name) : name(name) {}
const std::string &Employee::get_name() const { return name; }

std::ostream &operator<<(std::ostream &os, const Employee &e)
{
    e.print(os);
    return os;
}

void Employee::print(std::ostream &os) const
{
    os << "Employee: " << get_name() << std::endl;
}
