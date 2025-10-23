#include <cstdint>
typedef int8_t * INTPTR;
void function ( int8_t ** arrPar[ ] ) //UndCC_Violation
{
  int8_t ** obj2; //UndCC_Valid
  int8_t *** obj3; //UndCC_Violation
  INTPTR * obj4; //UndCC_Valid
  INTPTR * const * const obj5 = nullptr; //UndCC_Violation
  int8_t ** arr[ 10 ]; //UndCC_Valid
  int8_t ** ( *parr )[ 10 ]; //UndCC_Valid
  int8_t * ( **pparr )[ 10 ]; //UndCC_Valid
}
struct s
{
  int8_t * s1; //UndCC_Valid
  int8_t ** s2; //UndCC_Valid
  int8_t *** s3; //UndCC_Violation
};
struct s * ps1; //UndCC_Valid
struct s ** ps2; //UndCC_Valid
struct s *** ps3; //UndCC_Violation
int8_t ** ( *pfunc1 )( void ); //UndCC_Valid
int8_t ** ( **pfunc2 )( void ); //UndCC_Valid
int8_t ** ( ***pfunc3 )( void ); //UndCC_Violation
int8_t *** ( **pfunc4 )( void ); //UndCC_Violation

