// Custom

#include <h3.h>         // UndCC_Valid
#include <h1.hpp>       // UndCC_Valid
#include <h2.hxx>       // UndCC_Valid
#include <h4.cpp>       // UndCC_Violation
#include <h5.c>         // UndCC_Violation
#include "hello.h"      // UndCC_Valid
#include "goodbye.cpp"  // UndCC_Violation
#include "h"            // UndCC_Violation
#include "hpp"          // UndCC_Violation
#include "hxx"          // UndCC_Violation
