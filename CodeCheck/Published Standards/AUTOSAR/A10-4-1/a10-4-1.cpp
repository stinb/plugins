#include <iostream>

using namespace std;

// Base class
class Shape
{
public:
    // pure virtual function providing interface framework.
    virtual int getArea();
    static constexpr int x = {1};
};

// Derived classes
class Rectangle : public Shape // UndCC_Violation
{
public:
    int getArea()
    {
        return (x * x);
    }
};

class Triangle : public Shape // UndCC_Violation
{
public:
    int getArea()
    {
        return (x * x) / 2;
    }
};
