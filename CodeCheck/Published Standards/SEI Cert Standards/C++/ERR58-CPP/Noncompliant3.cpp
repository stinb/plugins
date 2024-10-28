extern int f() noexcept(false);
int i = f(); // UndCC_Violation

int main()
{
    // ...
}