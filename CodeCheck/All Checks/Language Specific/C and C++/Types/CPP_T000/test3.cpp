#include "test1.h"

// Classes and structs
class Class
{
	public:
    float32_t a; // UndCC_Valid
    float b;     // UndCC_Violation
};
struct Struct
{
  public:
    float32_t a; // UndCC_Valid
    float b;     // UndCC_Violation
};
