// Enums and pure virtual functions

enum Color { Red = 1, Green = 2, Blue = 3 };         // UndCC_Valid
enum class Status : int { Ok = 0, Fail = 1 };        // UndCC_Valid
enum Bits { B0 = 1 << 0, B1 = 1 << 1, B2 = 1 << 4 }; // UndCC_Valid
enum { MAX = 100 };                                  // UndCC_Valid
enum class Forward : int;

class Base
{
public:
  virtual void pure() = 0;                 // UndCC_Valid - pure specifier, not a value
  virtual void withParams(int a, int b) = 0; // UndCC_Valid
  virtual int constPure() const = 0;       // UndCC_Valid
  virtual int noexPure() noexcept = 0;     // UndCC_Valid
  virtual ~Base() {}
};

class Derived : public Base
{
public:
  void pure() override {}
  void withParams(int a, int b) override {}
  int constPure() const override { return a1; }
  int noexPure() noexcept override { return a1; }
  int a1;
};

void callPure(Base *b)
{
  b->withParams(0, 1); // UndCC_Violation - arguments are values
}

int add(int a, int b)
{
  return a + b;
}

enum Color globalColor; // an enum used as a type, so there is no enum body

int afterEnumType()
{
  return add(3, 4); // UndCC_Violation
}

int afterEnumerator()
{
  return Red + 5; // UndCC_Violation - outside the enum body
}

enum class Nested : int { N0 = int{5}, N1 = 6 }; // UndCC_Valid - braces in a value

enum Color returnsEnum() // an enum naming a return type, so the body is a function
{
  return (enum Color) add(1, 2); // UndCC_Violation
}

void enumInExpression()
{
  if (sizeof(enum Color) == 4) // UndCC_Violation
  {
    add(6, 7); // UndCC_Violation - the block is not an enum body
  }
}

enum Color arrayOfEnum[2]; // an enum naming an element type

int afterArrayOfEnum()
{
  return add(8, 9); // UndCC_Violation
}
