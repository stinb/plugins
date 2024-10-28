void foo()
{
    int i;
    int a[i];

    // i is modified twice in the same full expression
    i = ++i + 1; // UndCC_Violation

    // i is read other than to determine the value to be stored
    a[i++] = i; // UndCC_Violation
}