/* Custom */

#include <stdint.h>

typedef struct
{
  int32_t length;
} tFifo;

extern uint32_t text_length ( const char *str );
extern uint32_t raw_length ( char *str );

void f1 ( void )
{
  tFifo sFifo = { 4 };
  const tFifo *pFifo = &sFifo;
  tFifo *qFifo = &sFifo;

  int32_t a = ( ( tFifo * ) pFifo )->length;        /* UndCC_Violation - result used through -> */
  int32_t b = ( ( const tFifo * ) pFifo )->length;  /* UndCC_Valid - const kept */
  int32_t c = ( ( tFifo * ) qFifo )->length;        /* UndCC_Valid - no const to remove */
  ( void )a;
  ( void )b;
  ( void )c;
}

void f2 ( void )
{
  const uint8_t data[ 8 ] = { 0 };
  uint8_t plain[ 8 ] = { 0 };
  int32_t index = 2;

  ( void )text_length( ( char * )&data[ index ] );       /* UndCC_Violation - address of a const element */
  ( void )text_length( ( const char * )&data[ index ] ); /* UndCC_Valid */
  ( void )raw_length( ( char * )&plain[ index ] );       /* UndCC_Valid */
}

void f3 ( void )
{
  uint8_t bytes[ 8 ] = { 0 };
  const uint8_t *cbytes = bytes;
  uint8_t *mbytes = bytes;
  int32_t offset = 2;

  uint32_t key = *( ( uint16_t * )&( ( uint8_t * )cbytes )[ offset ] );        /* UndCC_Violation - inner cast */
  uint32_t key2 = *( ( const uint16_t * )&( ( const uint8_t * )cbytes )[ offset ] ); /* UndCC_Valid */
  uint32_t key3 = *( ( uint16_t * )&( ( uint8_t * )mbytes )[ offset ] );        /* UndCC_Valid */
  ( void )key;
  ( void )key2;
  ( void )key3;
}

void f4 ( void )
{
  volatile uint32_t reg = 0U;
  uintptr_t address = 16U;

  uint32_t value = *( uint32_t * )&reg;    /* UndCC_Violation - volatile removed */
  uint8_t *p = ( uint8_t * )address;       /* UndCC_Valid - integer to pointer */
  ( void )value;
  ( void )p;
}
