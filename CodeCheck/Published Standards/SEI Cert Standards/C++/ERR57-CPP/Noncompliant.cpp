#include <new>

struct SomeType
{
    SomeType() noexcept; // Performs nontrivial initialization.
    ~SomeType();         // Performs nontrivial finalization.
    void process_item() noexcept(false);
};

void f()
{
    SomeType *pst = new (std::nothrow) SomeType();
    if (!pst)
    {
        // Handle error
        return;
    }

    try
    {
        pst->process_item();    // UndCC_Violation
    }
    catch (...)
    {
        // Process error, but do not recover from it; rethrow.
        throw;
    }
    delete pst;
}
