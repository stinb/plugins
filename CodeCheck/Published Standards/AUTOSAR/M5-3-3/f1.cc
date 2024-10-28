// MISRA C++ 2008

class A;
void f ( A & a )
{
   &a; // uses built-in operator &
}
