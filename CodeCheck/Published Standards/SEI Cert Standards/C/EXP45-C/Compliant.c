void foo()
{
    int a;
    int b;
    int x;
    int y;
    int p;
    int q;
    int w;
    int v;
    char ch;
    char flag;

    if (a == b)
    {
        /* ... */
    }

    if ((a = b) != 0)
    {
        /* ... */
    }

    do
    { /* ... */
    } while (foo(), x == y);

    do
    { /* ... */
    } while (foo(), (x = y) != 0);

    for (; x; foo(), x = y)
    { /* ... */
    }

    do
    { /* ... */
    } while (x = y, p == q);

    while (x = y, p == q)
    {
        /* ... */
    }

    while ('\t' == ch || ' ' == ch || '\n' == ch)
    {
        /* ... */
    }

    if ((x = y) != 0)
    { /* ... */
    }

    if ((x = y))
    { /* ... */
    }

    if ((v == w) && flag)
    { /* ... */
    };

    if (((v = w) != 0) && flag)
    { /* ... */
    };

    if (foo1(x = y))
    { /* ... */
    }

    //x = a == b ? y : q; // potential violation
}

int foo1(int x)
{
    return 0;
}
