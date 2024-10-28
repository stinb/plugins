static int I = 12;

extern inline void func(int a)
{
    int b = a * I; // UndCC_Violation, extern function refers to a static variable
    /* ... */
}