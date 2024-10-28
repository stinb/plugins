// Test: Functions shall be declared at file scope

extern void okay1();
void okay2();

void func1() {
    extern void violation1(); // UndCC_Violation
    void violation2(); // UndCC_Violation
    int violation3(); // UndCC_Violation
}

class class1 {
    void okay1();
    void okay2() {};
    int okay3();
};

class A
{
};
void b1 ( )
{
    void f1 (); // UndCC_Violation - declaring a function in block scope
    A a ();     // UndCC_Violation - appears to declare an object with no
                // arguments to constructor, but it too declares a
                // function 'a' returning type 'A' and taking no
                // parameters.
}
