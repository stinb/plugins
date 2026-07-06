#include "string_lib.h"

// memset is declared in a header but a fortified library macro redirects
// the call to compiler builtins (__builtin___memset_chk, __builtin_object_size).
// Those are compiler intrinsics, not implicit declarations.
void test_fortified_memset( void ) {
    char buf[10];
    memset( buf, 0, sizeof( buf ) );  // UndCC_Valid
}

// Genuine implicit declaration: a function called with no prototype anywhere in
// the project. The strict parser records a declaration at the call site, so this
// is still flagged.
void test_implicit_declaration( void ) {
    int x = undeclaredHelper( 1, 2 );  // UndCC_Violation
}
