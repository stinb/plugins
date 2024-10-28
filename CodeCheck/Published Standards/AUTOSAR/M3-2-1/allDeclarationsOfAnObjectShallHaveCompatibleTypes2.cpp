// File b.cpp
extern int long aa;   
extern int yy;
extern int b [ 5 ]; // UndCC_Valid
int c; 
char f1 ( ); // UndCC_Violation
char f2 ( char ); // UndCC_Valid � not the same function as
                  // int32_t f2 ( int32_t )
