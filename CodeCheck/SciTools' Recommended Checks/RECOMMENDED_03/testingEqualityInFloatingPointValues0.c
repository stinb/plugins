/* MISRA04_13.3 */

int main() {
    float x, y;
    /* some calculations in here */
    if ( x == y )     /* UndCC_Violation */
    { /* ... */ }
    if ( x == 0.0f)   /* UndCC_Violation */

    if ( ( x <= y ) && ( x >= y ) )   /* UndCC_Violation */
    { /* ... */ }

    return 0;
}
