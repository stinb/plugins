struct S
{
    void mem_fn();
};

void f()
{
    S *s;
    s->mem_fn(); // UndCC_Violation
}