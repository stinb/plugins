/* C++ specific tests */

/* Extern declarations in C++ source file - non-compliant */
extern int global_x;                  // UndCC_Violation - extern object in source file
extern void free_func(void);          // UndCC_Violation - extern free function

/* Class definitions in source file - compliant */
class MyClass {                       // UndCC_Valid
public:
    void method(void);                // UndCC_Valid - member function declaration
    int getValue(void) const;         // UndCC_Valid - const member function
    virtual void vfunc(void);         // UndCC_Valid - virtual member function
    virtual void pvfunc(void) = 0;    // UndCC_Valid - pure virtual
private:
    void priv_method(void);           // UndCC_Valid - private member function
    int data;                         // UndCC_Valid - member data
};

/* Member function definitions - compliant */
void MyClass::method(void) {}         // UndCC_Valid - member function definition
int MyClass::getValue(void) const     // UndCC_Valid - const member definition
{
    return data;
}
void MyClass::vfunc(void) {}          // UndCC_Valid - virtual member definition
void MyClass::priv_method(void) {}    // UndCC_Valid - private member definition

/* Free function definition - compliant */
void defined_func(void) {}            // UndCC_Valid - function definition

/* Static free function - compliant */
static void internal_func(void);      // UndCC_Valid - static function declaration
static void internal_func(void) {}    // UndCC_Valid - static function definition

/* Block scope extern in C++ - non-compliant */
void cpp_block_scope(void)
{
    extern int block_var;             // UndCC_Violation - block scope extern
    (void)block_var;
}
