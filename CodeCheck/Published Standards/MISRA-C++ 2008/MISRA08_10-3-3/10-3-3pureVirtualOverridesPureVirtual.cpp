class A_1033
{
public:
	virtual void foo_1033 ( ) = 0; // foo declared pure
};

class B_1033 : public A_1033
{
public:
	virtual void foo_1033 ( ) // foo defined
	{
	}
};

class C_1033: public B_1033
{
public:
	virtual void foo_1033 ( ) = 0; // UndCC_Violation � foo re-declared pure
};
