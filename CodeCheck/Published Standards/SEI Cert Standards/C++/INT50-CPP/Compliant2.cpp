enum class EnumType
{
    First,
    Second,
    Third
};

void f(int intVar)
{
    EnumType enumVar = static_cast<EnumType>(intVar);
}