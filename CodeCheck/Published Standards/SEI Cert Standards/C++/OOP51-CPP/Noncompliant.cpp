#include "Employee.h"
#include "Manager.h"

void f(Employee e)
{
    std::cout << e;
}

int main()
{
    Employee coder("Joe Smith");
    Employee typist("Bill Jones");
    Manager designer("Jane Doe", typist);

    f(coder);
    f(typist);
    f(designer); // UndCC_Violation
}