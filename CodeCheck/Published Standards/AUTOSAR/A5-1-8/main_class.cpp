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

float main_class::get_g1()
{
	return g1;
}

void main_class::set_g1(float f_value)
{
	g1 = f_value;
}

long double main_class::get_g2()
{
	return g2;
}

void main_class::set_g2(long double ld_value)
{
	vector<int> v {4, 1, 3, 5, 2, 3, 1, 7};
	int x = 5;
	v.erase(remove_if(v.begin(), v.end(), [x](int n) { return n < x; }), v.end());
	
	g2 = ld_value;
}

void main_class::check_value(wchar_t string5[])
{
	wchar_t string32[] = L"GHI";
	cout << "wchar_t";
}

void main_class::abssort(float* x, unsigned n) {
    sort(x, x + n,
        [](float a, float b) {
						auto c1 = []() {}; // UndCC_Violation
            return (a < b);
        }
    );
}
