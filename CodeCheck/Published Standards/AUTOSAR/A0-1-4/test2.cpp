// Custom

#include <iostream>
using namespace std;

class base
{
public:
    virtual void print(int z) // UndCC_Valid since function is virtual
    {
        cout << "print base class" << endl;
    }

    void show()
    {
        cout << "show base class" << endl;
    }
};

class derived : public base
{
public:
    void print(int y) // UndCC_Valid
    {
        y = 0;
        cout << "print derived class" << endl;
    }

    void show(int x)  // UndCC_Violation
    {
        cout << "show derived class" << endl;
    }
};

void pass_class(derived *der)  // UndCC_Violation
{
}

void pass_class2(derived *der) // UndCC_Valid
{
    der->print(2);
}

int main()
{
    base *bptr;
    derived d;
    bptr = &d;

    // virtual function, binded at runtime
    bptr->print(1);

    // Non-virtual function, binded at compile time
    bptr->show();
}
