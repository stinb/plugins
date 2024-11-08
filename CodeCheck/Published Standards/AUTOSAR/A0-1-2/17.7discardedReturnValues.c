typedef unsigned short uint16_t;

uint16_t func ( uint16_t para1 )
{
	return para1;
}

func1() { // Missing specifier defaults to int
    int a = 0;
    return a;
}

uint16_t x;
void discarded ( uint16_t para2 )
{
	func ( para2 ); /* UndCC_Violation - value discarded */
	( void ) func ( para2 ); /* UndCC_Valid */
	x = func ( para2 ); /* UndCC_Valid */
    func1(); // UndCC_Violation

}

