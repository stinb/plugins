// Custom

int main()
{
  /* good escapes */
  char *st = "\a \b \f \n \r \t \v \' \" \? \xff \\";
  char end = '\0';
  /* bad escapes */
  char *x = "\012 \G"; // UndCC_Violation
  // char *y = "\xll"; // Analysis error
  // char end = '\N';  // Analysis error
  return 1;
}
