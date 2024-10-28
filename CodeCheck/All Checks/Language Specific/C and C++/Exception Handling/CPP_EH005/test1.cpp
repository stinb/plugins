void F8(); // UndCC_Violation(1) - missing noexcept specification
void F9(); // UndCC_Violation - missing noexcept specification
void F10() // UndCC_Valid - directly throws
{
    throw;
}

void F11() // UndCC_Valid - indirectly throws
{
    try
    {
        F8();
    }
    catch (...)
    {

    }
    F9();
    F10();
}

class C3
{
    public:
        C3()  // UndCC_Violation(1) - missing noexcept specification
        {
        }

        ~C3() // UndCC_Violation(1) - missing noexcept specification
        {
        }
};
