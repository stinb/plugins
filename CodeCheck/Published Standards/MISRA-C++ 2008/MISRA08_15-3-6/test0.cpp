// MISRA C++ 2008

// classes used for exception handling
class B { };
class D: public B { };
int main() {
    try
    {
        // ...
    }
    catch ( D &d ) // UNDCC_Valid – Derived class caught before base class
    {
        // ...
    }
    catch ( B &b ) // UNDCC_Valid – Base class caught after derived class
    {
        // ...
    }
    // Using the classes from above ...
    try
    {
        // ...
    }
    catch ( B &b )
    {
        // ...
    }
    catch ( D &d ) // UNDCC_Violation – Derived class will be caught above
    {
        // Any code here will be unreachable,
        // breaking Rule 0–1–1
    }
}
