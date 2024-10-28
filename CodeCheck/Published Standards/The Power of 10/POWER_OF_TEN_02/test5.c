int fn(int x)
{
    if (x % 2)
        return 0;

    return 1;
}

void foo(int n)
{
    int ctr = 0;

    for (ctr = 0; ctr < n; ctr++) // UndCC_Violation - n is modified
    {
        // ...
        if (ctr < n)
            n++;
        // ...
    }

    for (ctr = 0; ctr < n; ctr++) // UndCC_Violation - ctr is modified
    {
        // ...
        if (ctr < n)
            ctr = 0;
        // ...
    }

    for (ctr = 0; ctr < fn(n); ctr++) // UndCC_Violation - limit is not clear
    {
        // ...
    }

    while (ctr < 10) // UndCC_Violation - no upper bound limit, ctr is not modified inside the loop
    {
        n += 1;
    }
}
