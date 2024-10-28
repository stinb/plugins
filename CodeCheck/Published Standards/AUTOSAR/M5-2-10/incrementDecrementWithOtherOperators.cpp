#include <iostream>
extern void fn2(int, int);

void fn()
{
    int x = 0;
    int y = 0;
    int z = 0;
    int *xp;

    x = ++y + 10;   // UndCC_Violation
    y++;           // UndCC_Valid
    z = x-- + y;    // UndCC_Violation
    y = x + y++;     // UndCC_Violation
    --z;           // UndCC_Valid
    x = --x - ++y;  // UndCC_Violation

    for (x = 0; x < y + z; x--) // UndCC_Valid
    {
        std::cout << x;
    };

    fn2(y + 1, x++); // UndCC_Valid
}

