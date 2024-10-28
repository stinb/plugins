// Custom

#include "util.h"

class A
{
public:
    UtilType getValue ( );
    UtilType setValue ( UtilType const & );
};
void f1 ( A & a1, A & a2 )
{
    a1.getValue ( ) && a2.setValue ( 0 ); // Short circuiting may occur
}
// UtilType operator , ( UtilType const &, UtilType const & ); // Violation except in MISRA23_16.5.1
bool operator && ( UtilType const &, UtilType const & ); // UndCC_Violation
bool operator || ( UtilType const &, UtilType const & ); // UndCC_Violation
void f2 ( A & a1, A & a2 )
{
    a1.getValue ( ) && a2.setValue ( 0 ); // Both operands evaluated
}
