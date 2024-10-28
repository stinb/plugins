void throwing_func() noexcept(false);

void f1()
{
    throwing_func();
}

int main()
{
    try
    {
        f1();
    }
    catch (...)
    {
        // Handle error
    }
}