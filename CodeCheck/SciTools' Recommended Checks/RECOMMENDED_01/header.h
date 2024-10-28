int foo(); /* undefined prototype */
int fun();
int notfun ();
int violation() { // UndCC_Violation
	return fun() * 100;
}

char * violationPtr; // UndCC_Violation
class class3 {
  int memberobj;
  void okay1();
  void inline1() {}; // UndCC_Violation
};
