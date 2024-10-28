// Custom

// classes used for exception handling
class B { };
class D: public B { };
class F: public D { };
int y() {
  try
  {
    // ...
  }
  catch ( D &d ) // UndCC_Valid - Derived class caught before base class
  {
    // ...
  }
  catch ( B &b ) // UndCC_Valid - Base class caught after derived class
  {
    // ...
  }
}

int z() {
  // Using the classes from above ...
  try
  {
    // ...
  }
  catch ( B &b ) // will catch derived classes as well
  {
    // ...
  }
  catch ( D &d ) // UndCC_Violation - Derived class will be caught above
  {
                 // Any code here will be unreachable,
                 // breaking Rule 0-1-1
  }
}

int x() {
  // Using the classes from above ...
  try
  {
    // ...
  }
  catch ( B &b ) // will catch derived classes as well
  {
    // ...
  }
  catch ( F &f ) // UndCC_Violation - Derived class will be caught above
  {
                 // Any code here will be unreachable,
                 // breaking Rule 0-1-1
  }
  catch ( D &d ) // UndCC_Violation - Derived class will be caught above
  {
	             // Any code here will be unreachable,
	             // breaking Rule 0-1-1
  }
}
