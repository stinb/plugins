#include "main_class.hpp"

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

long double main_class::get_g2() // UndCC_Violation
{
	return g2;
}

void main_class::set_g2(long double ld_value) // UndCC_Violation
{
	g2 = ld_value;
}
