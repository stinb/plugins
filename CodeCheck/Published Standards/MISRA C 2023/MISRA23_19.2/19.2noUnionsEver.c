#include <stdint.h>
uint32_t zext ( uint16_t s )
{
	union // UndCC_Violation
	{
		uint32_t ul;
		uint16_t us;
	} tmp; // UndCC_Violation

	tmp.ul = 5;
	tmp.us = s;

	return tmp.ul; /* unspecified value */
}
