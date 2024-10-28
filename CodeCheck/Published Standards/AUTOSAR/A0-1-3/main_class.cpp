#include <vector>
#include <algorithm>
#include <iostream>
#include "main_class.hpp"

using namespace std;

main_class::main_class()
{
	
}

main_class::main_class(float f_value,long int li_value)
{
	g1 = f_value;
	g4 = li_value;
}

float main_class::get_g1() // UndCC_Valid, private member function is used
{
	return g1;
}

void main_class::set_g1(float f_value) // UndCC_Valid, never used but declared as public
{
	g1 = f_value;
}

long double main_class::get_g2() // UndCC_Valid, never used but declared as public
{
	return g2;
}

void main_class::set_g2(long double ld_value) // UndCC_Valid, never used but declared as public
{
	vector<int> v {4, 1, 3, 5, 2, 3, 1, 7};
	int x = 5;
	v.erase(remove_if(v.begin(), v.end(), [x](int n) { return n < x; }), v.end());
	
	g2 = ld_value * (long double)get_g1();
}

void main_class::check_value(wchar_t string5[]) // UndCC_Valid, never used but declared as public
{
	wchar_t string32[] = L"GHI";
	cout << "wchar_t";
}

void main_class::abssort(float* x, unsigned n) // UndCC_Valid, never used but declared as public
{ 
    sort(x, x + n,
        [](float a, float b) {
            []() {}; // UndCC_FalseNeg
						auto c1 = []() {};
            return (a < b);
        }
    );
}

void main_class::protected_get_g8(int i_value)  // UndCC_Valid, never used but declared as public
{
	cout << (g8 * i_value);
}

void main_class::private_set_g3(double d_value) // UndCC_Violation, private member function never used
{
	g3 = d_value;
}
