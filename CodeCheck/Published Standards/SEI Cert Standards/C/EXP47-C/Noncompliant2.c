#include <stdio.h>
#include <stdarg.h>

int vout(int max, ...);

typedef int numeric_day;

int main(void)
{
  vout(2, "Sat", "Sun");
  printf("\n");
  vout(3, "Mon", "Tues", "Wed");
}

int vout(int max, ...)
{
  va_list arg_ptr;
  va_list args_copy;
  int args;
  char *day;
  va_start(arg_ptr, max);
  va_copy(args_copy, arg_ptr);
  args = 0;
  while(args < max)
  {
    day = va_arg(arg_ptr, numeric_day); // UndCC_Violation
    printf("Day: %s\n", day);
    args++;
  }
  va_end(arg_ptr);
}
