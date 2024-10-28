int non_static;
static int m;
int n; // UndCC_Violation - only used in this file so it should be set as static
int y; // UndCC_Violation - only used in foo3 so it should be defined inside the function

void foo(int a)
{
    int ctr;

    for (ctr = 0; ctr < a; ctr++)
    {
        // ...
        if (n == a)
            n++;
    }

    m = 0;
}

void foo2(int b)
{
    int ctr;
    int x = 5; // UndCC_FalseNeg - should be defined inside if statement, but
               // without human knowledge, it may be a counter/accumulator

    for (ctr = 0; ctr < b; ctr++)
    {
        // ...
        if (n % b)
        {
            while (x < 5)
            {
                x--;
                n--;
            }

            x = 5;
        }
    }

    m = 1;
}

void foo3(int c)
{
    if (c == 0)
    {
        for (y = 0; y < 5; y++)
        {
            // ...
        }
    }

    non_static = 0;
}
