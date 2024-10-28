// Unreachable loop termination because of an early continue

enum { LIMIT = 8 };

typedef struct node_t {
  struct node_t *next;
} node_t;

void skip_with_continue(node_t *node)
{
  int i = 0;
  int limit = 8;
  while (node) { // UndCC_Valid - continue doesn't skip the loop termination
    node = node->next;
    i++;
    if (i >= limit)
      break;
  }

  i = 0;
  while (node) { // UndCC_Valid - continue doesn't skip the loop termination
    node = node->next;
    i++;
    if (node)
      for (int j = 0; j < 10; j++) // UndCC_Valid - normal for loop
        continue;
    if (i >= limit)
      break;
  }

  i = 0;
  while (node) { // UndCC_Violation - continue skips the loop termination
    node = node->next;
    i++;
    if (node)
      continue;
    if (i >= limit)
      break;
  }

  i = 0;
  while (node) { // UndCC_Violation - continue skips the loop iteration
    node = node->next;
    if (i >= limit)
      break;
    if (node)
      continue;
    i++;
  }
}
