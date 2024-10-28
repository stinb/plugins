#include <stdbool.h>

static int error = 0;

static int unchecked(int x, int y) // UndCC_Violation - unchecked parameter
{
  return 5 * x + 2 * y;
}

static int partially_checked_by_if(int x, int y) // UndCC_Violation - unchecked parameter
{
  if (!x) {
    error = -1;
    return -1;
  }
  return 5 * x + 2 * y;
}

static int checked_by_if(int x, int y)
{
  if (!x || y <= 0) {
    error = -1;
    return -1;
  }
  return 5 * x + 2 * y;
}

static int helper_function_checked_by_conditional(int x)
{
  return (x > 0) ? (x << 1) : x;
}

static int checked_by_if_after_passive_code(int x, int y)
{
  int z = 3;
  z = helper_function_checked_by_conditional(z);
  if (x && y > 0 && z)
    return 5 * x + 2 * y;
  z = x;
  error = -1;
  return -1;
}

static int checked_by_if_after_control_flow(int x, int y) // UndCC_Violation - unchecked parameter
{
  int z = 3;
  z = helper_function_checked_by_conditional(z);
  if (z) {
    if (x && y > 0)
      return 5 * x + 2 * y;
  }
  z = x;
  error = -1;
  return -1;
}

static int not_needed_for_bool_c(bool x, _Bool y)
{
  return !x || !y;
}

static int partially_checked_by_conditional(int x, int y) // UndCC_Violation - unchecked parameter
{
  return x ? (5 * x + 2 * y) : -1;
}

static int checked_by_conditional(int x, int y)
{
  return (x && y > 0) ? (5 * x + 2 * y) : -1;
}

static int checked_by_conditional_after_passive_code(int x, int y)
{
  int z = 3;
  z = helper_function_checked_by_conditional(z);
  return (x && y > 0) ? (5 * x + 2 * y) : -1;
}

static int checked_by_conditional_after_control_flow(int x, int y) // UndCC_Violation - unchecked parameter
{
  int z = 3;
  z = helper_function_checked_by_conditional(z);
  if (z) {
    return (x && y > 0) ? (5 * x + 2 * y) : -1;
  }
  z = x;
  error = -1;
  return -1;
}
