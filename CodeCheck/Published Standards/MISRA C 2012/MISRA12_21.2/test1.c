// Custom

#ifndef _MY_FILE
#define _MY_FILE   // UndCC_Violation: starts with '_'


int free(void *pArg, int len) {  // UndCC_Violation: free is a standard function
  int __test; // UndCC_Violation: starts with "__"
  //...
}
#endif
