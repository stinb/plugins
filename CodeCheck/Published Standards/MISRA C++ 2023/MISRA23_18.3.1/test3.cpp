void logError(char const *message);

int main() // handler may throw
{
    try
    {
        // Program code
    }
    catch (...) // Catch-all handler
    {
        logError("Unexpected"); // UndCC_Violation, potentially throwing function may lead to an exception propagating from main
    }
}
