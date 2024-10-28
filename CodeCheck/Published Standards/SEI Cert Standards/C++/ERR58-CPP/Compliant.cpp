struct S
{
    S() noexcept(false);
};

S &globalS()
{
    try
    {
        static S s;
        return s;
    }
    catch (...)
    {
        // Handle error, perhaps by logging it and gracefully terminating the application.
    }
    // Unreachable.
}