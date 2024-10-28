typedef struct node_t
{
  struct node_t *left;
  struct node_t *right;
} node_t;

static char hello[6] = "Hello";

static char* get_hello(void)
{
  return (char *) hello;
}

static char** get_hello_ptr(void)
{
  return (char **) &hello;
}

static void double_dereference(void)
{
  node_t node_y = { 0, 0 };
  node_t node_z = { 0, 0 };
  node_t node_x = { &node_y, &node_z };

  node_t *node_x_ptr = &node_x;

  int zero = 0;
  int *ptr_to_zero = &zero;
  int zeroes[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  char **hello_ptr = (char **) &hello;

  (void) *hello;              // UndCC_Valid
  (void) hello[0];            // UndCC_Valid
  (void) hello[*ptr_to_zero]; // UndCC_Valid
  (void) hello[zeroes[0]];    // UndCC_Valid

  (void) (hello_ptr)[0][0];   // UndCC_Violation
  (void) **hello_ptr;         // UndCC_Violation
  (void) *(hello_ptr)[0];     // UndCC_Violation
  (void) ((hello_ptr)[0])[0]; // UndCC_Violation
  (void) *((hello_ptr)[0]);   // UndCC_Violation
  (void) (*hello_ptr)[0];     // UndCC_Violation
  (void) *(*hello_ptr);       // UndCC_Violation

  (void) (&hello)[0][0];     // UndCC_Violation
  (void) (&((hello)))[0][0]; // UndCC_Violation
  (void) **&hello;           // UndCC_Violation
  (void) *(&hello)[0];       // UndCC_Violation
  (void) ((&hello)[0])[0];   // UndCC_Violation
  (void) *((&hello)[0]);     // UndCC_Violation
  (void) (*&hello)[0];       // UndCC_Violation
  (void) *(*&hello);         // UndCC_Violation

  (void) get_hello_ptr()[0][0];     // UndCC_Violation
  (void) **get_hello_ptr();         // UndCC_Violation
  (void) *get_hello_ptr()[0];       // UndCC_Violation
  (void) (get_hello_ptr())[0][0];   // UndCC_Violation
  (void) ((get_hello_ptr())[0])[0]; // UndCC_Violation
  (void) *(get_hello_ptr()[0]);     // UndCC_Violation
  (void) (*get_hello_ptr())[0];     // UndCC_Violation
  (void) *(*get_hello_ptr());       // UndCC_Violation

  (void) node_x_ptr->left;   // UndCC_Valid
  (void) (*node_x_ptr).left; // UndCC_Valid

  (void) node_x_ptr->left->left;     // UndCC_Violation
  (void) *(node_x_ptr->left);        // UndCC_Violation
  (void) (*node_x_ptr).left->left;   // UndCC_Violation
  (void) (*(*node_x_ptr).left).left; // UndCC_Violation
}
