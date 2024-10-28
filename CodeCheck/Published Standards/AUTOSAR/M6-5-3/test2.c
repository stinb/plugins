typedef struct node
{
  int value;
  struct node *next;
} node_t;

void fn ( node_t *node )
{
  for ( node_t *current = node; current; current = current->next )
    current->value <<= 1; // UndCC_Valid
}
