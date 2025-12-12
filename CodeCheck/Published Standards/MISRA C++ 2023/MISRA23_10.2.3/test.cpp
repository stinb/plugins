#include <cstdint>
#include <string>

enum E
{
    e1a,
    e1b
};

enum Other
{
    e2a
};

void g(int32_t i);

void f(E e)
{
    E e2 = e;       // UndCC_Valid - assignment to the same type
    int32_t i1 = e; // UndCC_Valid - assignment to a large enough integer

    e == e1a; // UndCC_Valid
    e < e1b;  // UndCC_Valid
    e == e2a; // UndCC_Violation - second operand of a different type
    e + 1;    // UndCC_Violation - addition

    g(e); // UndCC_Valid - assignment to large enough integer

    switch (e) // UndCC_Violation - cases are not all enumerators of E
    {
    case e1b:
        return; // e1b is an enumerator of E
    case e2a:
        return; // e2a is not an enumerator of E
    }

    auto s = sizeof(e + 1); // UndCC_FalsePos (automatic violation when used as operand to arithemitc operator)
                            // Unevaluated context - rule does not apply

    E e3 = static_cast<E>(0); // UndCC_Violation
    auto a1 = "QWERTY";

    a1[e1a];     // UndCC_Valid - index operator
    *(a1 + e1a); // UndCC_Violation

    std::string a2{a1};
    a2[e1a]; // UndCC_Valid - assignment to a large enough integer (size_t)
}