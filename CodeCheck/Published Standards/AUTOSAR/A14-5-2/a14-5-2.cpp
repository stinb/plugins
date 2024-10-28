template<typename T>
class A
{
public:
	enum State // UndCC_Violation: member doesn't depend on template parameter
	{
		State1,
		State2
	};

	State GetState();
};

class B_Base
{
public:
	enum State // UndCC_Valid: not a member of a class template
	{
		State1,
		State2
	};
};

template<typename T>
class B : B_Base
{
public:
	State GetState();
};
