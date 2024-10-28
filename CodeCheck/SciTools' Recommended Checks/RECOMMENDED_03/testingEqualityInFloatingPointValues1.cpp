// MISRA08_6-2-2

#include <cmath>
#include <limits>

int main() {
    float x, y;
    if ( x == y )    // UndCC_Violation
    if ( x == 0.0f ) // UndCC_Violation

    if ( ( x <= y ) && ( x >= y ) ) // UndCC_Violation
    if ( ( x < y ) || ( x > y ) )   // UndCC_Violation

    if ( fabs ( x - y ) <=
        std::numeric_limits<float>::epsilon( ) ) // UndCC_Valid

    return 0;
}
