int classify(int value)
{
  int rc;
  if (value > 0) {
    rc = 1;
  }
#ifdef UNDEFINED_MACRO
  else if (value == 0) {
    rc = 0;
  }
#endif
  else {                          // UndCC_Valid - the else if is excluded, and the else follows the directive
    rc = -1;
  }

  if (value > 10) {
    rc = 2;
  }
  else if (value > 5) {
    rc = 3;
  }                               // UndCC_Violation - the only else is excluded
#ifdef UNDEFINED_MACRO
  else {
    rc = 4;
  }
#endif

  if (value > 20) {
    rc = 5;
  }
  else if (value > 15) {
    rc = 6;
  }
#ifndef UNDEFINED_MACRO
  else {                          // UndCC_Valid - the else is compiled
    rc = 7;
  }
#endif
  return rc;
}
