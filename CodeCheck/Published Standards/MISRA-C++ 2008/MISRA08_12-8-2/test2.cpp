#include "test2.h"

// The out-of-line definition is not reported; the declaration in test2.h is
OutOfLine &OutOfLine::operator=(OutOfLine const &rhs) // UndCC_Valid
{
    return *this;
}
