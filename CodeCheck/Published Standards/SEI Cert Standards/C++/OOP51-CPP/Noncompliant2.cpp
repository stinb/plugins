#include <iostream>
#include <string>
#include <vector>

#include "Employee.h"
#include "Manager.h"

void f(const std::vector<Employee> &v)
{
    for (const auto &e : v)
    {
        std::cout << e;
    }
}

int main()
{
    Employee typist("Joe Smith");
    std::vector<Employee> v{typist, Employee("Bill Jones"), Manager("Jane Doe", typist)}; // UndCC_Violation
    f(v);
}