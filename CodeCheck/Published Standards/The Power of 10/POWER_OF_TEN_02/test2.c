// Loop termination in loop

enum { LIMIT = 8 };

typedef struct node_t {
  struct node_t *next;
} node_t;

void while_fixed_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  int limit = 8;
  while (node) { // UndCC_Valid - interator and fixed limit in break condition
    node = node->next;
    i++;
    if (i >= limit)
      break;
  }

  i = 0;
  #define limit 8
  while (node) { // UndCC_Valid - interator and fixed limit in break condition
    node = node->next;
    i++;
    if (i >= limit)
      return;
  }
  #undef limit

  i = 0;
  while (node) { // UndCC_Valid - interator and fixed limit in break condition
    node = node->next;
    i++;
    if (i >= LIMIT) {
      int x = 0;
      break;
    }
  }
}

void while_complicated_fixed_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  while (node) { // UndCC_FalsePos - else is not checked
    node = node->next;
    i++;
    if (i < LIMIT)
      ;
    else
      break;
  }

  i = 0;
  while (node) { // UndCC_FalsePos - switch case is not checked
    node = node->next;
    i++;
    switch (i) {
      case LIMIT:
        return;
    }
  }
}

void while_modified_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  int limit = 8;
  while (node) { // UndCC_Violation - limit modified
    if (i == 4)
      limit = limit << 1;
    node = node->next;
    i++;
    if (i >= limit)
      break;
  }
}

void while_volatile_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  volatile int limit = 8;
  while (node) { // UndCC_Violation - limit is volatile
    node = node->next;
    i++;
    if (i >= limit)
      break;
  }
}

void while_no_upper_bound_with_termination(node_t *node)
{
  while (node) { // UndCC_Violation - no upper limit
    node = node->next;
  }
}

void do_while_fixed_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  int limit = 8;
  do { // UndCC_Valid - interator and fixed limit in break condition
    node = node->next;
    i++;
    if (i >= limit)
      break;
  }
  while (node);
}

void do_while_modified_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  int limit = 8;
  do { // UndCC_Violation - limit is modified
    if (i == 4)
      limit <<= 1;
    node = node->next;
    i++;
    if (i >= limit)
      break;
  } while (node);
}

void do_while_volatile_upper_bound_with_termination(node_t *node)
{
  int i = 0;
  volatile int limit = 8;
  do { // UndCC_Violation - limit is volatile
    node = node->next;
    i++;
    if (i >= limit)
      break;
  } while (node);
}

void do_while_no_upper_bound_with_termination(node_t *node)
{
  do { // UndCC_Violation - no limit
    node = node->next;
  } while (node);
}
