enum EnumType
{
    First,
    Second,
    Third
};

void f(int intVar)
{
    EnumType enumVar = static_cast<EnumType>(intVar);   // UndCC_Violation

    if (enumVar < First || enumVar > Third)
    {
        // Handle error
    }
}
