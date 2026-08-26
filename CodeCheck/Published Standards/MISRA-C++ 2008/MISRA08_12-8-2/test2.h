#ifndef TEST2_H
#define TEST2_H

// Abstract class whose copy assignment operator is defined out-of-line
class OutOfLine
{
public:
    virtual void f() = 0;
    OutOfLine &operator=(OutOfLine const &rhs); // UndCC_Violation
};

#endif
