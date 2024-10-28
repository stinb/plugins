void invalidDeclaration(int, double, long, ...); // UndCC_Violation
void validFunc(int a1, ...);

void validFunc(int a1, ...) {
}

typedef signed   int    int32_t;
void absolutelyInvalid(int32_t); // UndCC_Violation
void notInvalid(int32_t a);
void absolutelyInvalid(const int32_t); // UndCC_Violation

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

int
invalidFunc // UndCC_Violation
(
	/* comment */
	int
	/* comment */
);

int uglyButValidSpacing(int a,long double b,unsigned char c);

void otherParams(int a1, double, char /* blah */ *a2); // UndCC_Violation

int superUglyAndInvalid(int,int,int,int,int); // UndCC_Violation
