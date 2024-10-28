struct MultiThreadedFlags
{
    unsigned int flag1 : 2;
    unsigned int flag2 : 2;
};

MultiThreadedFlags flags;

void thread1()
{
    flags.flag1 = 1; // UndCC_Violation
}

void thread2()
{
    flags.flag2 = 2; // UndCC_Violation
}
