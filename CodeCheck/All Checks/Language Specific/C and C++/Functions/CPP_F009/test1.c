// Custom

void func_a ( int );  // UndCC_Violation - unnamed parameter
void func_a ( n ) { } // UndCC_Violation - not in prototype form

void ( *func_b ( void ( *x ) ( void ) ) ) ( void ( *y ) ( void ) );
void ( *func_c ( void ( * ) ( void ) ) ) ( void ( *y ) ( void ) ); // UndCC_Violation - unnamed parameter
void ( *func_d ( void ( *x ) ( void ) ) ) ( void ( * ) ( void ) ); // UndCC_Violation - unnamed parameter
void ( *func_e ( void ( *x ) ( ) ) ) ( void ( *y ) ( void ) );     // UndCC_Violation - not in prototype form
void ( *func_f ( void ( *x ) ( void ) ) ) ( void ( *y ) ( ) );     // UndCC_Violation - not in prototype form

void func_g ( int ( x ) ( int ) ) { } // UndCC_Violation - unnamed parameter

void func_h ( int x, int y );
void func_h ( int a, int b ) { }

void func_i ( int a, int b ) {
  func_j( ( void ( * ) ( int ) ) 0 ); // UndCC_Violation - unnamed parameter
}
