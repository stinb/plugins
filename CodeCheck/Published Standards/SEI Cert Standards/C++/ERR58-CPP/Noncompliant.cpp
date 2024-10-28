struct S
{
    S() noexcept(false);
};

static S globalS; // UndCC_Violation