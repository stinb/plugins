// Custom

#include <string.h>

enum { MESSAGE_SIZE = 64 };

static char message_a[MESSAGE_SIZE] = ""; // UndCC_Violation - Global declaration should be local in hello_message_a
static char message_b[MESSAGE_SIZE] = "";

static void hello_message_a(void)
{
  strcpy_s(message_a, MESSAGE_SIZE, "Hi\n");
}

static void hello_message_b(void)
{
  strcpy_s(message_b, MESSAGE_SIZE, "Hi\n");
}

static void goodbye_message_b(void)
{
  strcpy_s(message_b, MESSAGE_SIZE, "Bye\n");
}

static void goodbye_message_c(void)
{
  static char message_c[MESSAGE_SIZE] = ""; // UndCC_Valid - Used in this scope
  strcpy_s(message_c, MESSAGE_SIZE, "Bye\n");
}

static void goodbye_message_d(void)
{
  char message_d[MESSAGE_SIZE] = ""; // UndCC_Valid - Used in this scope
  strcpy_s(message_d, MESSAGE_SIZE, "Bye\n");
}

static void if_statements(int input)
{
  int a = input; // UndCC_Valid - Used in multiple lower scopes

  int b = 3; // UndCC_Violation - Used in 1 lower scope
  int c = 4; // UndCC_Violation - Used in 1 lower scope
  if (a > 0) {
    a += b + b;
  }
  else {
    a += c + c + c;
  }

  {
    int d = 3; // UndCC_Violation - Used in 1 lower scope
    if (a > 0) {
      a += d + d;
    }
  }

  {
    int d = 3; // UndCC_Valid - Used in multiple lower scopes
    if (a > 0) {
      a += d + d;
    }
    else if (a > 0) {
      a += d;
    }
    else if (a == 0) {
      a += d;
    }

    if (a > 0) {
      int e = 3; // UndCC_Valid - Used in this scope
      a += e + e;
    }
  }
}

static void switch_statement_a(int input)
{
  int a = input; // UndCC_Valid - Used in this scope
  int b = 3;     // UndCC_Violation - Used in 1 lower scope
  switch (a) {
    case 1:
      a += b + b;
      break;
    case 2:
      break;
    default:
      break;
  }
}

static void switch_statement_b(int input)
{
  int a = input; // UndCC_Valid - Used in this scope
  int b = 3;     // UndCC_Violation - Used in 1 lower scope
  switch (a) {
    case 1:
      break;
    case 2:
      break;
    default:
      a += b + b;
      break;
  }
}

static void switch_statement_c(int input)
{
  int a = input; // UndCC_Valid - Used in this scope
  int b = 3;     // UndCC_Valid - Used in multiple lower scopes
  switch (a) {
    case 1:
      a += b;
      break;
    case 2:
      break;
    default:
      a += b;
      break;
  }
}

static void loop_a(int input)
{
  int i = input;  // UndCC_Valid - Exception because the immediate lower scope is a loop
  int limit = 10; // UndCC_Valid - Exception because the immediate lower scope is a loop
  while (1)
  {
    i += 2;
    if (i >= limit)
      break;
  }
}

static void loop_b(int input)
{
  int i = input;  // UndCC_Violation - Should be declared in the lower 'if' scope
  int limit = 10; // UndCC_Violation - Should be declared in the lower 'if' scope
  if (1) {
    while (1)
    {
      i += 2;
      if (i >= limit)
        break;
    }
  }
}

static void loop_c(int input)
{
  if (1) {
    int i = input;  // UndCC_Valid - Exception because the immediate lower scope is a loop
    int limit = 10; // UndCC_Valid - Exception because the immediate lower scope is a loop
    while (1)
    {
      i += 2;
      if (i >= limit)
        break;
    }
  }
}

static void loop_d(int input)
{
  if (1) {
    int i = input;  // UndCC_Violation - Should be declared in the lower 'if' scope
    int limit = 10; // UndCC_Violation - Should be declared in the lower 'if' scope
    if (1) {
      while (1)
      {
        i += 2;
        if (i >= limit)
          break;
      }
    }
  }
}

static void loop_e(int input)
{
  if (1) {
    if (1) {
      int i = input;  // UndCC_Valid - Exception because the immediate lower scope is a loop
      int limit = 10; // UndCC_Valid - Exception because the immediate lower scope is a loop
      while (1)
      {
        i += 2;
        if (i >= limit)
          break;
      }
    }
  }
}

static void loop_f(int input)
{
  for (int i = input; i < 10; i++) { // UndCC_Valid - Used in this scope

  }

  int i; // UndCC_Valid - Used in this scope
  for (i = input; i < 10; i++) {

  }
}

static void loop_g(int input)
{
  int i = input; // UndCC_Valid - Used in this scope
  while (i < 20) {
    i += 3;
    if (i < 0)
      break;
  }
}

static void loop_h(int input)
{
  int i = input; // UndCC_Valid - Exception because the immediate lower scope is a loop
  do {
    i += 3;
    if (i < 0)
      break;
  } while (i < 20);
}
