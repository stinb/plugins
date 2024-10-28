#include <iostream>
#include <string>

class Noncopyable
{
    Noncopyable(const Noncopyable &) = delete;
    void operator=(const Noncopyable &) = delete;

protected:
    Noncopyable() = default;
};

class Employee_2 : Noncopyable
{
    // Remainder of the definition is unchanged.
    std::string name;

protected:
    virtual void print(std::ostream &os) const
    {
        os << "Employee_2: " << get_name() << std::endl;
    }

public:
    Employee_2(const std::string &name) : name(name) {}
    const std::string &get_name() const { return name; }
    friend std::ostream &operator<<(std::ostream &os, const Employee_2 &e)
    {
        e.print(os);
        return os;
    }
};

class Manager_2 : public Employee_2
{
    const Employee_2 &assistant; // Note: The definition of Employee_2 has been modified.

    // Remainder of the definition is unchanged.
protected:
    void print(std::ostream &os) const override
    {
        os << "Manager_2: " << get_name() << std::endl;
        os << "Assistant: " << std::endl
           << "\t" << get_assistant() << std::endl;
    }

public:
    Manager_2(const std::string &name, const Employee_2 &assistant) : Employee_2(name), assistant(assistant) {}
    const Employee_2 &get_assistant() const { return assistant; }
};

// If f() were declared as accepting an Employee_2, the program would be
// ill-formed because Employee_2 cannot be copy-initialized.
void f(const Employee_2 &e)
{
    std::cout << e;
}

int main()
{
    Employee_2 coder("Joe Smith");
    Employee_2 typist("Bill Jones");
    Manager_2 designer("Jane Doe", typist);

    f(coder);
    f(typist);
    f(designer);
}