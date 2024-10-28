// Dereference in macro, pointer type abstraction, function pointer

typedef struct {
  int x;
  int y;
} point_t;

static point_t origin = { 0, 0 };
static char *string;
static int nums[16];

static point_t* get_origin_ptr(void)
{
  return &origin;
}

static char* get_string(void)
{
  return string;
}

static int *get_nums(void)
{
  return nums;
}

static void deref_macros(void)
{
  point_t *point_ptr = &origin;

  #define MULTIPLY_X_AND_Y_0(point) (point).x * (point).y
  MULTIPLY_X_AND_Y_0(origin);

  #define DECL_POINT_PTR_0(name) point_t *name
  DECL_POINT_PTR_0(point);

  #define DECL_NUMS_0(name, length) int name[length]
  DECL_NUMS_0(small_nums, 8);

  #define EXPR_GENERIC_PTR_0(ptr) ((void *) ptr)
  EXPR_GENERIC_PTR_0(point_ptr);

  #define NOTHING_0(x) x
  NOTHING_0(*point_ptr);
  *NOTHING_0(point_ptr);
  NOTHING_0(point_ptr->x);
  NOTHING_0(point_ptr)->y;
  NOTHING_0(string[0]);
  NOTHING_0(string)[0];

  #define RESET_POINT_PTR_A_0(point_ptr) { point_t *_ptr = (point_ptr); _ptr->x = 0; _ptr->y = 0; } // UndCC_Violation(0)
  RESET_POINT_PTR_A_0(point_ptr);

  #define RESET_POINT_PTR_B_0(point_ptr) { point_t *_ptr = (point_ptr); _ptr->x = 0; _ptr->y = 0; } // UndCC_Violation(0)
  RESET_POINT_PTR_B_0(get_origin_ptr());

  #define CLEAR_STRING_A_0(char_ptr) { *(char_ptr) = 0; } // UndCC_Violation(0)
  CLEAR_STRING_A_0(string);

  #define CLEAR_STRING_B_0(char_ptr) { *(char_ptr) = 0; } // UndCC_Violation(0)
  CLEAR_STRING_B_0(get_string());

  #define GET_NUM_A_0(nums, n) (nums)[n] // UndCC_Violation(0)
  GET_NUM_A_0(nums, 1);

  #define GET_NUM_B_0(nums, n) (nums)[n] // UndCC_FalseNeg
  GET_NUM_B_0(get_nums(), 1);

  #define RESET_X_POINT_PTR_A_0(point_ptr) { (point_ptr)->x = 0; } // UndCC_Violation(0)
  RESET_X_POINT_PTR_A_0(point_ptr);

  #define RESET_X_POINT_PTR_B_0(point_ptr) { (point_ptr)->x = 0; } // UndCC_Violation(0)
  RESET_X_POINT_PTR_B_0(get_origin_ptr());

  #define DEREF_FUNCTIONAL_A_0(point_ptr) *(((point_ptr))) // UndCC_Violation(0)
  DEREF_FUNCTIONAL_A_0(point_ptr);

  #define DEREF_FUNCTIONAL_B_0(point_ptr) *(((point_ptr))) // UndCC_Violation(0)
  DEREF_FUNCTIONAL_B_0(get_origin_ptr());

  #define DEREF_POINT_PTR_0 *point_ptr // UndCC_Violation(0)
  DEREF_POINT_PTR_0;
}

static void pointer_in_type_definition(void)
{
  typedef point_t* point_ptr_t; // UndCC_Violation

  typedef int ints[16]; // UndCC_Violation

  typedef struct node_t {
    struct node_t *left;
    struct node_t *right;
  } node_t;

  typedef void *(function_t) (void *);
  typedef void *(*function_ptr_t) (void *); // UndCC_Violation
}

extern void *function (void *);
static void *(*function_ptr) (void *);      // UndCC_Violation
static void *(**function_ptr_ptr) (void *); // UndCC_Violation
