void invalidDeclaration(int a, double b, long c, ...); // UndCC_Violation
void invalidFunc(int a1, ...);  // UndCC_Violation

void invalidFunc(int a1, ...) { // UndCC_Violation
}

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
