#include <stdlib.h>

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

struct s
{
	uint16_t len;
	uint16_t other[10];
	uint32_t data[ ]; 	/* UndCC_Violation - flexible array member 			*/
} str;

struct s *copy ( struct s *s1 )
{
	struct s *s2;
	/* Omit malloc ( ) return check for brevity 							*/
	s2 = malloc ( sizeof ( struct s ) + ( s1->len * sizeof ( uint32_t ) ) );
	*s2 = *s1; 			/* Only copies s1->len 								*/
	return s2;
}
