// Test: Functions shall be declared at file scope

extern void okay1();
void okay2();

void func1() {
	extern void violation1(); // UndCC_Violation
	void violation2(); // UndCC_Violation
	int violation3(); // UndCC_Violation
}

void b1 ( )
{
	void f1 (); // UndCC_Violation - declaring a function in block scope
}
