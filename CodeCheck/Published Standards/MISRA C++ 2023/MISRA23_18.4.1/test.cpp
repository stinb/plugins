#include <cstdint>
#include <exception>
#include <string>

class C1
{
public:
    C1() {}  // UndCC_Valid - never used at non-local scope
    ~C1() {} // UndCC_Valid - noexcept by default
};

class C2
{
public:
    C2() {} //  Not noexcept - see declaration of c2 below
    C2(const C2 &) {}
    C2(C2 &&other) {}             // UndCC_Violation - move constructor
    C2 &operator=(C2 &&other);    // UndCC_Violation - move assignment
    ~C2() noexcept(true) {}       // UndCC_Valid
    friend void swap(C2 &, C2 &); // UndCC_Violation - function named swap
};

C2 c2a; // UndCC_Violation, construction is non-local

class C3
{
public:
    C3() {}                  // UndCC_Valid - c3 in foo not in non-local scope
    ~C3() noexcept(false) {} // UndCC_Violation
};

class MyException : public std::exception // UndCC_Violation - implicit copy
{                                         // constructor is noexcept( false )
public:
    MyException(std::string const &sender); // Rule does not apply
    const char *what() const noexcept override;
    std::string sender;
};

void foo()
{
    static C3 c3; // UndCC_Valid - constructed on first call to foo
    throw MyException("foo");
}

extern "C"
{
    void f(void (*func)());
}

void exit_handler1();
void exit_handler2() noexcept;

int main()
{
    try
    {
        const int32_t result1 = std::atexit(exit_handler1); // UndCC_Violation - exit_handler1 not noexcept
        const int32_t result2 = std::atexit(exit_handler2); // UndCC_Valid
        C1 c1;
        foo(); // Any exception thrown will be caught below
    }
    catch (...)
    {
    }

    f([]() {}); // UndCC_Violation - function passed to extern "C"
}
