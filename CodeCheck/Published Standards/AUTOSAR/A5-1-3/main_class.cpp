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
	int i = 0;
	int j = 0;
	auto lambda3 = [&i, &j]() { ++i; ++j; }; // UndCC_Valid
	
	return g2;
}

void main_class::set_g2(long double ld_value)
{
	g2 = ld_value;
}
