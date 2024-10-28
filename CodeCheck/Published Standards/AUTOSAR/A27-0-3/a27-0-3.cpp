// $Id: A27-0-3.cpp 311495 2018-03-13 13:02:54Z michal.szczepankiewicz $

#include <fstream>
#include <string>

int main(void)
{
std::fstream f("testfile");

f << "Output";
std::string str1;
f >> str1; // UndCC_Violation

f << "More output"; // UndCC_Violation
std::string str2;
f.seekg(0, std::ios::beg);
f >> str2; // UndCC_Valid
return 0;
}
