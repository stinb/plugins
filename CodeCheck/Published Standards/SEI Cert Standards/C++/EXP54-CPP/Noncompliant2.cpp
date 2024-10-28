struct B
{
};

struct D1 : virtual B
{
};
struct D2 : virtual B
{
};

struct S : D1, D2
{
};

void f(const B *b) {}

void g()
{
    S *s = new S;
    // Use s
    delete s;

    f(s); // UndCC_Violation
}