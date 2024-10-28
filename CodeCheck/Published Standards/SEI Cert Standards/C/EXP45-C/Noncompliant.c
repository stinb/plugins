void foo_n()
{
    int a;
    int b;
    int x;
    int y;
    int p;
    int q;
    int v;
    int w;
    char ch;
    char flag;

    if (a = b) // UndCC_Violation
    {
        /* ... */
    }

    do
    {                       /* ... */
    } while (foo(), x = y); // UndCC_Violation

    do
    {                       /* ... */
    } while (x = y, p = q); // UndCC_Violation

    while (x = y, p = q) // UndCC_Violation
    {
        /* ... */
    }

    while (ch = '\t' || ch == ' ' || ch == '\n') // UndCC_Violation
    {
        /* ... */
    }

    // while ('\t' = ch || ' ' == ch || '\n' == ch) // Parse error
    // {
    //     /* ... */
    // }

    if ((v = w) && flag) // UndCC_Violation
    {                    /* ... */
    }

    //x = a == b ? y = p : q;   // potential violation
    //x = a == b ? y : (p = q); // potential violation
}
