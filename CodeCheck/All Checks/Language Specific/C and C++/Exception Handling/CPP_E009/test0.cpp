// MISRA C++ 2008

class Exception { };

// foo does not have an exception specification, so can propagate
// exceptions of any type, including int
void foo ( )
{
   throw ( 21 );
}

// goo specifies that it will only throw exceptions of type Exception.
// If foo throws an int the function unexpected() is called, which may
// terminate the program
void goo ( ) throw ( Exception )
{
   foo ( ); // UndCC_Violation - int is not listed in the
            // throw specification
}
