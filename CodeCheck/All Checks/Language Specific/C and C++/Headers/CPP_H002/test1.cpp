//    '
#include <fi'le.h>   // UndCC_Violation
#include "fi'le.h" // UndCC_Violation

//    "
#include <fi"le.h>   // UndCC_Violation
// #include "fi\"le.h"  // Valid except for MISRA23_19.2.3

//    /*
#include <fi/*le.h>  // UndCC_Violation
#include "fi/*le.h"  // UndCC_Violation

//    //
#include <fi//le.h>  // UndCC_Violation
#include "fi//le.h"  // UndCC_Violation

//    /
#include <fi/le.h>   // UndCC_Valid
#include "fi/le.h"   // UndCC_Valid

//    *
#include <fi*le.h>   // UndCC_Valid
#include "fi*le.h"   // UndCC_Valid
