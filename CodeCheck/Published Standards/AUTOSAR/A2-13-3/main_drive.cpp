#include <iostream>
#include "main_class.hpp"

using namespace std;

double long i1 = 0.01F; 
wchar_t string4[] = L"GHI"; // UndCC_Violation

int f1(float *p1, double p2)
{
	return int(p2);
}

void f4(int);
void f5(int *);
void fn() noexcept
{
	int i_value = NULL; 
	float f1{0.1f};
	double f2{0.1};
	long double f3{0.1L};
	int f4(NULL); 
	int f5(i_value); 
}

int main()
{
	fn();
        
	int h1 = f1(NULL, 0.01); 
	
	main_class start_engine;
	cout << "g1 value : "<< start_engine.get_g1();
	
	char16_t string1[] = u"ABC"; // UndCC_Valid
	char32_t string2[] = U"DEF"; // UndCC_Valid
	
	unsigned char buffer[12];
	switch (buffer[0]) 
	{
		case 0x01:
			cout << "buffer is 1";
			break;
		case 0xAA:
			cout << "buffer is 170";
			break;
		case 0xff:
			cout << "buffer is 255";
			break;
	}
        
	return 0;
}

// g++ -Wconversion-null -std=c++14 main_drive.cpp main_class.cpp main_class.hpp -o main_drive.exe
