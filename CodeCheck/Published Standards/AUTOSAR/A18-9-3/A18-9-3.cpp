// $Id: A18-9-3.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <utility>

class A
{
	// Implementation
};

void F1()
{
	const A a1{};
	A a2 = a1;            // UndCC_Valid - copy constructor is called
	A a3 = std::move(a1); // UndCC_Violation - copy constructor is called
	                      // implicitly instead of move constructor
}

int main()
{
	return 0;
}
