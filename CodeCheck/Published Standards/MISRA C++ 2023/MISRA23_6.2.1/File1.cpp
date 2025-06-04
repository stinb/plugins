// File1.cpp
#include "File1.h"
#include "File3.h"

void f1()
{
    h(42); // Nested call in h is to int32_t overload of f
}