void logError(char const *message);

int main() // UndCC_Valid
{
    try
    {
        // Program code
    }
    catch (...) // Catch-all handler
    {
        try
        {
            logError("Unexpected");
        }
        catch (...)
        {
            // Logging also threw
        }
    }
}
