extern inline void func(void)
{
    static int I = 12; // UndCC_Violation
    /* Perform calculations which may modify I */
}