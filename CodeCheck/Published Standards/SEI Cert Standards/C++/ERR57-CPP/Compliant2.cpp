struct SomeType
{
    SomeType() noexcept; // Performs nontrivial initialization.
    ~SomeType();         // Performs nontrivial finalization.

    void process_item() noexcept(false);
};

void f()
{
    SomeType st;
    try
    {
        st.process_item();
    }
    catch (...)
    {
        // Process error, but do not recover from it; rethrow.
        throw;
    } // After re-throwing the exception, the destructor is run for st.
} // If f() exits without throwing an exception, the destructor is run for st.