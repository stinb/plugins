#include <stdbool.h>

static int three;

static bool get_true(void)
{
    return true;
}

static int get_two(void)
{
    return 2;
}

static void return_values(void)
{
  bool b;
  int x, y;

  b = get_true();
  b = !b || b;

  x = get_two();
  if (x > 0)
    y = x;

  x = get_two();
  y = (x > 0) ? x : 0;

  x = get_two(); // UndCC_Violation - unchecked return value
  y = x;

  y = get_two();
  if (y == 0)
    y += 2;

  for (int i = 0; i < 10; i++) {
    x = get_two();
    if (x > 0)
      y = x;

    x = get_two();
    y = (x > 0) ? x : 0;

    x = get_two(); // UndCC_Violation - unchecked return value
    y = x;

    y = get_two();
    if (y == 0)
      y += 2;
  }
}

static int *get_three_ptr(void)
{
    return &three;
}

static void ptr_return_values(void)
{
  int *three_ptr;

  three_ptr = get_three_ptr();
  if (three_ptr != 0)
    *three_ptr = 2;

  three_ptr = get_three_ptr();
  if (three_ptr != (void *) 0)
    *three_ptr = 2;

  three_ptr = get_three_ptr();
  if (three_ptr)
    *three_ptr = 2;

  three_ptr = get_three_ptr();
  if (true && three_ptr && true)
    *three_ptr = 2;

  three_ptr = get_three_ptr(); // UndCC_Violation - pointer return value not checked for null
  if (*three_ptr == 3)
    *three_ptr = 2;
}
