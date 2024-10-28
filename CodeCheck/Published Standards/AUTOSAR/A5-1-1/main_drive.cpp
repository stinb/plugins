#include <iostream>
#include "main_class.hpp"

using namespace std;

double long i1 = 0.01F;  // UndCC_Valid - assignment
wchar_t string4[] = L"GHI"; // UndCC_Valid - assignment
int iMax = 10; // UndCC_Valid - assignment
float iFloatMax=0.1F; // UndCC_Valid - assignment

int f1(float *p1, double p2)
{
	return int(p2);
}

void f4(int);
void f5(int *);
void fn() noexcept
{
	int i_value = NULL;
	float f1{0.1f}; // UndCC_Valid - assignment
	double f2{0.1}; // UndCC_Valid - assignment
	long double f3{0.1L}; // UndCC_Valid - assignment
	long double f6 { 0.1L }; // UndCC_Valid - assignment
	int f4(NULL);
	int f5(i_value);
	//int f9(15,13); // error compiling - expression list treated as compound expression in initializer
	int f10 ( 3); // UndCC_Valid - assignment
}

int main()
{
	fn();

	int h1 = f1(NULL, 0.01);

	main_class start_engine;
	cout << "g1 value : "<< start_engine.get_g1();

	char16_t string1[] = u"ABC"; // UndCC_Valid - assignment
	char32_t string2[] = U"DEF"; // UndCC_Valid - assignment

	unsigned char buffer[12]; // UndCC_Valid - array size
	switch (buffer[0]) // UndCC_Violation
	{
		case 0x01: // UndCC_Violation
			cout << "buffer is 1";
			break;
		case 0xAA: // UndCC_Violation
			cout << "buffer is 170";
			break;
		case 0xff: // UndCC_Violation
			cout << "buffer is 255";
			break;
	}

	for (int i = 0; i < 10; ++i) // UndCC_Violation
	{
		cout << "iter " << i << ':' << '\n'; // UndCC_Valid by exception
	}

	for (int i = 0; i < iMax; ++i) // UndCC_Valid
	{
          cout << "iter " << i << ':' << '\n'; // UndCC_Valid by exception
	}

	return 0; // UndCC_Violation
}

// g++ -Wconversion-null -std=c++14 main_drive.cpp main_class.cpp main_class.hpp -o main_drive.exe
