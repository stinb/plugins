extern void func(int i, int j);

void f(int i)
{
    func(i++, i); // UndCC_Violation
}