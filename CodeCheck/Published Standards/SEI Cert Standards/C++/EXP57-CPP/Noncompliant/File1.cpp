// File1.cpp
#include "File1.h"
#include "File2.h"

void f()
{
    B *v = get_d();
    g(reinterpret_cast<class D *>(v)); // UndCC_Violation(cast) in EXP57-CPP
}