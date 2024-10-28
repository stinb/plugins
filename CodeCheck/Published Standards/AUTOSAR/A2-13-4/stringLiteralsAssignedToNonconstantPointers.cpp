//% $Id: A2-13-4.cpp 307578 2018-02-14 14:46:20Z michal.szczepankiewicz $

int main(void)
{
	char* nc1 = "AUTOSAR";  // UndCC_Violation
	char nc2[] = "AUTOSAR"; // UndCC_Valid with A2-13-4, non-compliant with A18-1-1
	char nc3[8] = "AUTOSAR"; // UndCC_Valid with A2-13-4, non-compliant with A18-1-1
	nc1[3] = 'a'; // undefined behaviour
	const char* c1 = "AUTOSAR"; // UndCC_Valid
	const char c2[] = "AUTOSAR"; // UndCC_Valid with A2-13-4, non-compliant with A18-1-1
	const char c3[8] = "AUTOSAR"; // UndCC_Valid with A2-13-4, non-compliant with A18-1-1
	//c1[3] = ’a’; //compilation error

	return 0;
}
