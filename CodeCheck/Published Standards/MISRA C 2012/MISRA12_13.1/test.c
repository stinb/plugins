#include <stdint.h>
volatile uint16_t v1;
void f ( void )
{
  uint16_t a[ 2 ] = { v1, 0 };  // UndCC_Violation - volatile access is persistent side effect

}
void g ( uint16_t x, uint16_t y )
{
  uint16_t a[ 2 ] = { x + y, x - y }; // UndCC_Valid
}
uint16_t x = 0u;
extern void p ( uint16_t a[ 2 ] );


uint32_t get_value(void) {
  static uint32_t value = 0;
  return value++;
}
void h ( void )
{
  p ( ( uint16_t[ 2 ] ) { x++, x++ } ); // UndCC_Violation
  p ( ( uint16_t[ 2 ] ) { get_value(), get_value() } ); // UndCC_Violation
}
int main() {
  uint32_t arr[2] = {get_value(), get_value()};  // UndCC_Violation
  uint32_t x = 0;
  uint32_t arr1[2] = {x = 5, x *= 2};  // UndCC_Violation
  uint32_t arr2[2] = {x++, x--};  // UndCC_Violation
  uint32_t arr3[2] = {(uint32_t)(x++), 5};  // UndCC_Violation
}

