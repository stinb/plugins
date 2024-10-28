struct A
{ /* ... */
};
struct B
{ /* ... */
};

void g(A *, B *);
void f()
{
    g(new A, new B); // UndCC_Violation
}