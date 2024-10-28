// Pointer type abstraction and multi-pointer declaration

#include <utility>

typedef struct{
  int x;
  int y;
} point_t;

static void pointer_in_type_definition_cpp()
{
  using point_ptr_t = point_t*; // UndCC_Violation
}

void multi_pointer_template()
{
  std::pair<int, int> template_obj_0;
  std::pair<int, int> **template_obj_1;     // UndCC_Violation(1,2)
  std::pair<int *, int*> template_obj_2;
  std::pair<int **, int> template_obj_3;    // UndCC_Violation(1,2)
  std::pair<int, int **> template_obj_4;    // UndCC_Violation(1,2)
  std::pair<int *, int *> *template_obj_5;
  std::pair<int *, int *> **template_obj_6; // UndCC_Violation(1,2)
}
