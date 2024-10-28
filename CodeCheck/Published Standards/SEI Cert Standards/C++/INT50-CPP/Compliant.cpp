enum EnumType
{
    First,
    Second,
    Third
};

void f(int intVar)
{
    if (intVar < First || intVar > Third)
    {
        // Handle error
    }
    EnumType enumVar = static_cast<EnumType>(intVar); // UndCC_Violation due to limitation
}
