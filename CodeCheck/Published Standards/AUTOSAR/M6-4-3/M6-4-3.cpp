void foo()
{
    int x;
    int y;

    switch (x) // UndCC_Violation
    {
    case 0:
        y = 1;
        break; // break is required here
    case 1:
        y = 2; // Non-empty clause, break required (Non-compliant)
    case 2:
    case 3:
        break; // break is required here
    default:   // default clause is required
        break; // break is required here, in case a future
               // modification turns this into a case clause
    }

    switch (x) // UndCC_Valid
    {
    case 0:
        y = 1;
        break; // break is required here
    case 1:    // empty clause, break not required
    case 2:
        break; // break is required here
    default:   // default clause is required
        break; // break is required here, in case a future
               // modification turns this into a case clause
    }
}