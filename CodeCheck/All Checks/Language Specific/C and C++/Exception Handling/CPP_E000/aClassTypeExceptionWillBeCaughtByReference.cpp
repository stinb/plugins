// base class for exceptions
class ExpBase
{
public:
  virtual const char_t *who ( )
  {
    return "base";
  };
};
class ExpD1: public ExpBase
{
public:
  virtual const char_t *who ( )
  {
    return "type 1 exception";
  };
};
class ExpD2: public ExpBase
{
public:
  virtual const char_t *who ( )
  {
    return "type 2 exception";
  };
};

int main() {
  try
  {
    // ...
    throw ExpD1 ( );
    // ...
    throw ExpBase ( );
  }
  catch ( ExpBase &b ) // UndCC_Valid � exceptions caught by reference
  {
    // ...
    b.who(); // "base", "type 1 exception" or "type 2 exception"
             // depending upon the type of the thrown object
  }
  // Using the definitions above ...
  catch ( ExpBase b ) // UndCC_Violation - derived type objects will be
                      // caught as the base type
  {
    b.who(); // Will always be "base"
    throw b; // The exception re-thrown is of the base class,
             // not the original exception type
  }
}
