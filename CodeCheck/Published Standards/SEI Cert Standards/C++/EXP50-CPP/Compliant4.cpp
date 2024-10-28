extern void func(int i, int j);

void f(int i)
{
    i++;
    func(i, i);
}