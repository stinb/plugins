void f()
{
    int *array = new int[10];
    // ...
    delete array;       // UndCC_Violation
}
