int a; /* UNDCC_Valid - Globals are automatically initialized to zero */
int b = 0; /* UNDCC_Valid */

int main() {
  a = 1;
  b = 1;
  char c; /* UNDCC_Violation */
  char d = 'a'; /* UNDCC_Valid */
  c = 'b';
  d = 'b';
}

// Uninitialized struct array in C++
typedef struct {
  int x;
  int y;
} Event;

void f(Event *events);

void test_struct_array() {
  Event events[2]; // UndCC_Violation
  for (int i = 0; i < 2; i++)
    f(events + i);
}

void test_single_struct() {
  Event e; // UndCC_Violation
}

void test_initialized_struct() {
  Event e = {0, 0}; // UndCC_Valid
}

// Class with user-defined constructor — default construction is meaningful
class Widget {
  int val;
public:
  Widget() : val(0) {}
  Widget(int v) : val(v) {}
  int get() const { return val; }
};

void test_class_with_constructor() {
  Widget w; // UndCC_Valid - default constructor initializes val
  Widget w2(5); // UndCC_Valid
}

// Brace initialization
void test_brace_init() {
  Event e{}; // UndCC_Valid
  int x{}; // UndCC_Valid
}
