#include <cstdint>

int f(int a, int b);
int main() {
    int x;
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    
    x = a + b;       // acceptable
    x = (a + b);      // UndCC_Violation
    
    x = a * -1;      // acceptable
    x = a * (-1);     // UndCC_Violation
    x = sizeof(a);
    
    x = a + b + c;                   // acceptable, but care needed
    x = f (8 + (a + b), c );          // UndCC_Violation
    x = ( a == b ) ? a : ( a - b );
    if ( a && b && c )               // acceptable
      x = ( a + b ) - ( c + d );
    x = ( a * 3 ) + c + d;
    x = static_cast< uint16_t > ( a ) + b; // no need for cast
    
    return 0;
}
