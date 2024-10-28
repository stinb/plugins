//% $Id: A2-13-1.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <string>
void F()
{
	const std::string a = "\k";             // UndCC_Violation
	const std::string b = "\n";             // UndCC_Valid
	const std::string c = "\U0001f34c";     // UndCC_Valid

	const std::string d = "Hello\world";    // UndCC_Violation
	const std::string e = "Hello\\world";   // UndCC_Valid
	const std::string f = "Hello\\\world";  // UndCC_Violation
	const std::string g = "Hello\\\\world"; // UndCC_Valid
}
