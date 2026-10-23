// Custom

void f3 ( int x, int y )
{
    switch ( x ) // UndCC_Violation(c) - throw ends the clause
    {
    case 0:
        y = 1;
        break;
    default:
        throw 1;
    }
}
