// Custom

struct S {
  void mem_fn();
};

void test0()
{
  S *s = new S;
  delete s;
  s->mem_fn(); // UndCC_Violation - access after deletion
}

void test1()
{
  S *s;
  s->mem_fn(); // UndCC_Violation - access before creation
  s = new S;
  delete s;
}

void test2()
{
  S *s;
  if (0)
    s = new S; // Dead code
  s->mem_fn(); // UndCC_FalseNeg - access before creation
  if (s != nullptr)
    delete s;
}

void test3()
{
  S *s;
  if (0)
    delete s;  // Dead code
  s->mem_fn(); // UndCC_FalsePos - access during lifetime
  if (s != nullptr)
    delete s;
}

int *test4() {
  int i = 12;
  return &i;
}

int *test5() {
  int i = 12;
  int *j = &i;
  return j;
}
