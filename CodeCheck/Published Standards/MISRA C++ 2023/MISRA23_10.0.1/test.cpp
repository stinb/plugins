#include <functional>
#include <map>
#include <utility>
#include <vector>

std::map<char, char> f()
{
    return std::map<char, char>();
}

int i1; int _j1;                                 // UndCC_Valid

int i2, *j2;                                    // UndCC_Violation

int *i3,                                        // UndCC_Violation
    &j3 = i2;

struct point
{
    int x, y;                                   // UndCC_Violation
};

void g()
{
    std::map<char, char> map = f();

    auto [loc, inserted] =
        map.insert(std::make_pair('A', 'a'));   // UndCC_Valid - structured binding
}

std::map<int, int> m1;                          // UndCC_Valid - template comma is not a declarator comma
std::map<int, int> m2, m3;                      // UndCC_Violation

struct outer
{
    struct inner
    {
        int a, b;                               // UndCC_Violation - nested class member
    };

    int c;                                      // UndCC_Valid
};

void h()
{
    int arr[3], other;                          // UndCC_Violation - array declarator plus plain declarator
    int brace1{1}, brace2{2};                   // UndCC_Violation - brace-init does not break grouping
}

void range_for()
{
    std::vector<int> container{1, 2, 3};        // UndCC_Valid - template plus brace-init, single declarator

    for (auto &item : container)                // UndCC_Valid
    {
        (void)item;
    }
}

void qualifiers()
{
    static int s1, s2;                          // UndCC_Violation
    const int c1 = 1, c2 = 2;                    // UndCC_Violation
}

void lambdas()
{
    std::function<void()> fn1 = [](){}, fn2 = [](){}; // UndCC_Violation

    auto fn3 = []()                              // UndCC_Valid - single declarator
    {
        int a;                                   // UndCC_Valid
        int b;                                   // UndCC_Valid
    };
}

void pointer_levels()
{
    int *p1, **p2, ***p3;                        // UndCC_Violation
}

void function_pointers()
{
    void (*fp1)(int), (*fp2)(int);               // UndCC_FalseNeg - declarator nested in its own parens
}

template <int N>
void template_param_single()                    // UndCC_Valid - single non-type template parameter
{
}

template <typename T, int M>
void template_param_multi()                     // UndCC_Valid - template parameter list, not a variable declaration
{
}

int after_template1, after_template2;           // UndCC_Violation - must not be merged with a prior template parameter list
