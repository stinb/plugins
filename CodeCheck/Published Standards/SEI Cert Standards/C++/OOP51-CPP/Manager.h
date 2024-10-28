#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"

class Manager : public Employee
{
    Employee assistant;

protected:
    void print(std::ostream &os) const override;

public:
    Manager(const std::string &name, const Employee &assistant);
    const Employee &get_assistant() const;
};

#endif