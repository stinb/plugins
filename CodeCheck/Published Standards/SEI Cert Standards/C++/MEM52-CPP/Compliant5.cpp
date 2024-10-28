struct A
{ /* ... */
};
struct B
{ /* ... */
};

void g(A &a, B &b);
void f()
{
    A a;
    B b;
    g(a, b);
}