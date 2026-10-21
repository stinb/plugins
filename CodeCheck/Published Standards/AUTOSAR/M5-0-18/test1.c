// MISRA C 2012

#include <stdint.h>

void f1 ( void )
{
 int32_t a1[ 10 ];
 int32_t a2[ 10 ];
 int32_t *p1 = a1;

 if ( p1 < a1 ) /* UndCC_Valid */
 {
 }
 if ( p1 < a2 ) /* UndCC_Violation */
 {
 }
}

struct limits
{
 int32_t lwb;
 int32_t upb;
};

void f2 ( void )
{
 struct limits limits_1 = { 2, 5 };
 struct limits limits_2 = { 10, 5 };

 if ( &limits_1.lwb <= &limits_1.upb ) /* UndCC_Valid */
 {
 }
 if ( &limits_1.lwb > &limits_2.upb )   /* UndCC_Violation */
 {
 }
}

typedef struct
{
 uint8_t buf[ 64 ];
 uint8_t *cur;
 uint8_t *end;
} buffer_t;

void f3 ( void )
{
 buffer_t b1;
 uint8_t other[ 64 ];
 b1.cur = &( b1.buf[ 0 ] );
 b1.end = &b1.buf[ 64 ];
 uint8_t *cur_copy = b1.cur;
 uint8_t *copy_of_copy = cur_copy;

 if ( cur_copy < b1.buf ) /* UndCC_Valid */
 {
 }
 if ( copy_of_copy <= b1.end ) /* UndCC_Valid */
 {
 }
 if ( cur_copy < other ) /* UndCC_Violation */
 {
 }
}

extern uint32_t text_length ( const char *str );
extern int32_t parse_number ( const char *str, char **end );

void f4 ( int32_t *x, int32_t *y, int32_t **px, int32_t **py, const char *str )
{
 char buf[ 8 ];

 if ( *x < *y ) /* UndCC_Valid - values, not pointers */
 {
 }
 if ( x[ 0 ] < y[ 0 ] ) /* UndCC_Valid */
 {
 }
 if ( text_length(str) > (sizeof(buf) - 1U) ) /* UndCC_Valid */
 {
 }
 if ( *px < *py ) /* UndCC_Violation - still pointers after one dereference */
 {
 }
}

void f5 ( const char *text )
{
 char *end;
 char other[ 4 ];

 ( void )parse_number( text, &end );

 if ( end > text ) /* UndCC_Valid - end points into text */
 {
 }
 if ( end > other ) /* UndCC_Violation */
 {
 }
}

static char *skip_spaces ( char *str )
{
 char *cursor = str;

 while ( *cursor == ' ' )
 {
  cursor++;
 }

 return cursor;
}

static char *new_buffer ( char *str )
{
 static char fresh[ 16 ];

 ( void )str;
 return fresh;
}

void f6 ( void )
{
 char text[ 16 ];
 char *word = skip_spaces( text );
 char *copy = new_buffer( text );

 if ( word > text ) /* UndCC_Valid - returns a pointer into its argument */
 {
 }
 if ( copy > text ) /* UndCC_Violation - returns a pointer to another object */
 {
 }
}

int32_t count_back ();
int32_t is_before ();

void f7 ( void )
{
 char line[ 10 ];
 char first[ 4 ];
 char second[ 4 ];

 ( void )count_back( line, line + 9 );
 ( void )is_before( first, second );
}

int32_t count_back ( start, stop )
 char *stop, *start;
{
 int32_t n = 0;

 while ( stop > start ) /* UndCC_Valid - callers pass one buffer */
 {
  stop--;
  n++;
 }

 return n;
}

int32_t is_before ( a, b )
 char *a, *b;
{
 return a < b; /* UndCC_Violation - callers pass separate arrays */
}

static int32_t in_range ( const char *pos, const char *limit )
{
 return pos < limit; /* UndCC_Valid - every caller passes one buffer */
}

static int32_t is_after ( const char *p, const char *q )
{
 return p > q; /* UndCC_Violation - one caller passes separate arrays */
}

void f8 ( void )
{
 char msg[ 8 ];
 char one[ 4 ];
 char two[ 4 ];

 ( void )in_range( msg, msg + 8 );
 ( void )in_range( msg + 2, msg + 8 );
 ( void )is_after( one + 1, one );
 ( void )is_after( one, two );
}

struct holder
{
 char *data;
};

static char *data_of ( struct holder *h )
{
 return h->data;
}

void f9 ( void )
{
 struct holder h1;
 struct holder h2;
 char *d1 = data_of( &h1 );
 char *d2 = data_of( &h2 );
 char *d3 = data_of( &h1 );

 if ( d1 <= d3 ) /* UndCC_Valid - both from h1 */
 {
 }
 if ( d1 < d2 ) /* UndCC_Violation - different holders */
 {
 }
}
