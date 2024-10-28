class S
{
    int c;

public:
    S() : c(0) {}
    int f(int i) const { return i + c; }
};

void f()
{
    S s;
    int i = s.f(10);
}