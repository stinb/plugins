// AUTOSAR

#include <directory/headerfile.hpp>  // UndCC_Valid
#include <headerfile.hpp>            // UndCC_Valid
#include "directory/headerfile.hpp"  // UndCC_Valid
#include "headerfile.hpp"            // UndCC_Valid
// #include <directory/*.hpp>        // Compilation error
#include <header'file.hpp>           // UndCC_Violation
#ifdef __UNDERSTAND_FUZZY_PARSE__
#include <"headerfile.hpp">          // UndCC_Violation
#include <directory\\headerfile.hpp> // UndCC_Violation
#endif
