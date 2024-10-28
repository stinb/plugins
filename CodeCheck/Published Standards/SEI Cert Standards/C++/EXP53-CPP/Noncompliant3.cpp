class S
{
    int c;

public:
    int f(int i) const { return i + c; } // UndCC_Violation
};

void f()
{
    S s;
    int i = s.f(10);
}