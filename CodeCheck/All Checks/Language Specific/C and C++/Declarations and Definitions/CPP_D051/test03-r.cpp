#include <string>

// The typedef specifier
typedef int int1; // UNDCC_Valid
int typedef int2; // UNDCC_Violation

// Function specifiers
inline std::string x1 = "Hi";    // UNDCC_Valid
std::string inline x2 = "Hello"; // UNDCC_Violation

inline int f1(); // UNDCC_Valid
int inline f2(); // UNDCC_Violation

class A
{
    virtual void f3(); // UNDCC_Valid
    void virtual f4(); // UNDCC_Violation
};

// The friend specifier
class C
{
    friend void ff1(); // UNDCC_Valid
    void friend ff2(); // UNDCC_Violation
};

// The constexpr specifier
constexpr float x3 = 42.0; // UNDCC_Valid
float constexpr x4 = 64.0; // UNDCC_Violation

constexpr int f5(int x, int y); // UNDCC_Valid
int constexpr f6(int x, int y); // UNDCC_Violation

template <class T>
constexpr T f7 (T x, T y); // UNDCC_Valid
template <class T>
T constexpr f8(T x, T y);  // UNDCC_Violation

class E
{
    constexpr static int x = 0; // UNDCC_Valid
    static constexpr int y = 1; // UNDCC_Valid
    int static constexpr z = 2; // UNDCC_Violation
};

// Storage class specifiers
void f9() {
    register int x; // UNDCC_Valid
    int register y; // UNDCC_Violation

    static int x5; // UNDCC_Valid
    int static x6; // UNDCC_Violation

    thread_local static int x7; // UNDCC_Valid
    int thread_local extern x8; // UNDCC_Violation
}

class F {
    mutable int x; // UNDCC_Valid
    int mutable y; // UNDCC_Violation
};
