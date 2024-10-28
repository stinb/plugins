class A_1031
{
public:
	virtual void f1_1031 ( ) = 0; 	// f1 is pure
	virtual void f2_1031 ( ) = 0;	// f2 is pure
	virtual void f3_1031 ( ) { } 	// f3 is not pure
	virtual void f4_1031 ( ) = 0; 	// f4 is pure
	virtual ~A_1031(); 				// destructor
};

									// A_1031::f1_1031 is both pure and has a definition
void A_1031::f1_1031 ( )
{
}

									// A_1031::f4_1031 is both pure and has a definition
void A_1031::f4_1031 ( )
{
}


class B_1031 : public A_1031
{
public:
	virtual void f2_1031 ( ) { } 	// UndCC_Valid: f2 pure in A and
									// defined in B
	virtual void f3_1031 ( ) { } 	// UndCC_Violation: f3 defined in A and B
	virtual void f4_1031 ( ) = 0; 	// UndCC_Valid: f4 is pure in A and in B
	virtual ~B_1031(); 				// UndCC_Valid: destructor
};

									// UndCC_Valid by Exception - f4 defined in A but also declared pure in A
void B_1031::f4_1031 ( )
{
}


class C_1031 : public B_1031
{
public:
	virtual void f1_1031 ( ) { } 	// UndCC_Valid: f1 defined in A and C
									// but was pure in A
	virtual void f2_1031 ( ) { } 	// UndCC_Violation f2: defined in B and C
									// and not declared pure in B
	virtual void f4_1031 ( ) { } 	// UndCC_Valid by Exception: f4 defined in A
									// and B but also declared pure in A and B
};


class D_1031 : public C_1031
{
public:
	virtual void f1_1031 ( ) { } 	// UndCC_Violation f1: defined in C and D
									// as well as A
	virtual ~D_1031(); 				// UndCC_Valid: destructor
};


									// Call by dominance example
class V_1031
{
public:
virtual void foo_1031 ( )
{
}
};
class B1_1031 : public virtual V_1031
{
public:
virtual void foo_1031 ( ) 				// UndCC_Violation
{
}
};
class B2_1031 : public virtual V_1031
{
public:
void f1_1031 ( )
{
foo_1031(); 								// V::foo would appear to be the only
									// candidate to be called here
}
};
class D1_1031 : public B1_1031, public B2_1031
{
public:
void f2_1031 ( )
{
f1_1031();
}
};

int main_1031 () {
	
	B2_1031 b2_1031;
	b2_1031.f1_1031();						// calls V::foo by normal inheritance rules
	D1_1031 d_1031;
	d_1031.f2_1031();							// calls B2::f1 which now calls B1::foo
									// "by dominance"
	d_1031.f1_1031();							// also calls B1::foo "by dominance"
	
	return 0;
}
