#include <iostream>
#include <cstdint>
#include "main_class.hpp"

using namespace std;

double long i1 = 0.01F;
wchar_t string4[] = L"GHI";

static void f6() // UndCC_Violation, never used
{
}

namespace
{
	void f7() // UndCC_Violation, defined function never used
	{
	}
}

void notCalledButGlobal() // UndCC_Valid - Has global linkage
{}

int f1(float *p1, double p2) // UndCC_Valid, Line 41
{
	return int(p2);
}

void f4(int); 
void f5(int *); 
void fn() noexcept // UndCC_Valid
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
	cout << "g1 value : "<< start_engine.get_g2();
	
	char16_t string1[] = u"ABC";
	char32_t string2[] = U"DEF";
	
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
	
	for (int32_t i = 0; i < 10; ++i) 
	{
		cout << "iter " << i << ':' << '\n';
	}
	
	auto lambda1 = []() -> uint8_t { uint8_t ret = 0U; return ret; };

	auto lambda2 = []() {
		int n = [] (int x, int y) { return x + y; }(5, 4);
		return 0U;
	}; 
	
	auto lambda3 = []() -> int { 
		int ret = 10; 
		return ret; 
	};
	
	auto x = lambda1();
	auto y = lambda2();
	auto z = lambda3();
	
	return 0;
}

// g++ -Wconversion-null -std=c++14 main_drive.cpp main_class.cpp main_class.hpp -o main_drive.exe
