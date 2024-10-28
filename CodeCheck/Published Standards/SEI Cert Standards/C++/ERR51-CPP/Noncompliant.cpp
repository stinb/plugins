void throwing_func3() noexcept(false);

void f3()
{
    throwing_func3();   // UndCC_Violation
}

int main()
{
    f3();
}
