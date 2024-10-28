// Custom

static void else_if_0(char op)
{
  char *next; // UndCC_Valid - Used in multiple lower scopes

  if (op == '*')
    ;
  else if (op == '-')
    ;
  else if (op == '/')
    ;
  else if (op == '+') {
    (void) next;
    (void) next;
    (void) next;
  } else if (op == '?') {
    (void) next;
    (void) next;
  }
  else {
    if (op == '<') {

    }
    else if (op == '>') {

    }
    else {

    }
  }
}

static void else_if_1(char op)
{
  char *next; // UndCC_Violation - Used in 1 lower scope

  if (op == '*')
    ;
  else if (op == '-')
    ;
  else if (op == '/')
    ;
  else if (op == '+') {
    (void) next;
    (void) next;
    (void) next;
  } else if (op == '?') {

  }
  else {
    if (op == '<') {

    }
    else if (op == '>') {

    }
    else {

    }
  }
}

static void shadowing(int input)
{
  int a = input;

  int b = 3; // UndCC_Violation - Used in 1 lower scope
  switch (a) {
    case 1:
      a += b + b;
      break;
    case 2: {
      int b = 3; // UndCC_Violation - Used in 1 lower scope
      switch (a) {
        case 1:
          a += b + b;
          break;
        case 2:
          break;
        default:
          break;
      }
      break;
    }
    default:
      break;
  }
}

static void no_shadowing(int input)
{
  int a = input;

  int b = 3; // UndCC_Valid - Used in multiple lower scopes
  switch (a) {
    case 1:
      a += b + b;
      break;
    case 2:
      switch (a) {
        case 1:
          a += b + b;
          break;
        case 2:
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
