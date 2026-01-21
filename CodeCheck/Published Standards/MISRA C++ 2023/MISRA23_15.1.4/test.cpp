#include <cstdint>
#include <string>

class PersonClass
{
public:
    PersonClass(std::string const &name, int32_t age) : name{name}, age{age} // UndCC_Valid
    {
    }
    explicit PersonClass(int32_t age) : age{age} // UndCC_Valid - name is default constructed,
    {
    } // and income initialized to 1000
    explicit PersonClass(std::string const &name) : name{name} // UndCC_Violation - age not initialized
    {
        age = 18;
    }
    PersonClass() = default; // UndCC_Violation - age not initialized
private:
    std::string name;
    int32_t age;
    int32_t income = 1000;
};

class PersonAggregate
{
public:
    std::string name;
    int32_t age;
    int32_t income{1000};
};

void f()
{
    PersonAggregate p1;   // UndCC_Violation - age not initialized, even though
                          // name and income are initialized
    PersonAggregate p2{}; // UndCC_Valid - name is default constructed, and age is
} // initialized to 0, income to 1000

class Building // UndCC_Violation - height not initialized in the
{              // implicit default constructor
private:
    std::string name;

public:
    int32_t height;
};

class Base
{
    int32_t a;

public:
    explicit Base(int32_t a) : // UndCC_Valid
                               a{a}
    {
    }
};

class Derived : public Base
{
    int32_t b;

public:
    Derived() : // UndCC_Valid
                Base{0}, b{}
    {
    }
    using Base::Base; // UndCC_Violation - b not initialized by the
}; // synthesized constructor
