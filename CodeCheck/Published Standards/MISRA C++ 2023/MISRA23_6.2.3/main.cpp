// main.cpp
#include "b.h"
#include "c.h" // ODR violation
#include "d.h"
A<D> const a2{}; // OK - requires inclusion of d.h
