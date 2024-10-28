// Custom

void f ( char *p )
{
  *p++;   // UndCC_Violation
  (*p)++; // UndCC_Valid
}
