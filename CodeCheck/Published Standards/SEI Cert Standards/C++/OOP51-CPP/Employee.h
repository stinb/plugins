#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <string>

class Employee
{
    std::string name;

protected:
    virtual void print(std::ostream &os) const;

public:
    Employee(const std::string &name);
    const std::string &get_name() const;
    friend std::ostream &operator<<(std::ostream &os, const Employee &e);
};

#endif