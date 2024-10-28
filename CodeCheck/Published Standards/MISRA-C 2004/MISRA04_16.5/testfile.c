void invalidDeclaration();
void invalidFunc(); // UndCC_Violation

void invalidFunc() { // UndCC_Violation
}

void stillInvalidFunc( /* still crap */ );

void validDeclaration(void);
void validFunc(void);

void validFunc(void) {
}

void
validSpacing1
(
	/* comment */
	void
	/* comment */
);

void
validSpacing2
(
	/* comment */
	int
	/* comment */
	a
	/* comment */
);

int uglyButValidSpacing(int a,long double b,unsigned char c);

void otherParams(int a1, double, char /* blah */ *a2);
