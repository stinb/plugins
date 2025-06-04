// File2.cpp
#include "File2.h"
#include "File3.h"
void f2()
{
    h(42); // Nested call in h is to int64_t overload of f
}