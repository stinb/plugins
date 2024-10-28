#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Employee.h"
#include "Manager.h"

void f(const std::vector<std::unique_ptr<Employee>> &v)
{
    for (const auto &e : v)
    {
        std::cout << *e;
    }
}

int main()
{
    std::vector<std::unique_ptr<Employee>> v;

    v.emplace_back(new Employee("Joe Smith"));
    v.emplace_back(new Employee("Bill Jones"));
    v.emplace_back(new Manager("Jane Doe", *v.front()));

    f(v);
}