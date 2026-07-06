// Custom

#ifndef _MY_FILE
#define _MY_FILE   // UndCC_Violation: starts with '_'


int free(void *pArg, int len) {  // UndCC_Violation: free is a standard function
  int __test; // UndCC_Violation: starts with "__"
  //...
}

// Library-style macro (e.g. arm_neon.h) expands a reserved local at the call site
#define GET_VALUE(x) ({ int __ret = (x); __ret; })
int use_macro(void) {
  return GET_VALUE(5);  // UndCC_Valid: __ret comes from macro expansion
}
#endif
