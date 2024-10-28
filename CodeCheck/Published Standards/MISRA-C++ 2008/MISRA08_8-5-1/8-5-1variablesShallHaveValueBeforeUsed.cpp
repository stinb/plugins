class C_851
{
public:
	C_851 ( ) : m_a_851( 10 ), m_b_851( 7 ) // UndCC_Valid
	{
	}
	C_851 ( int a_851 ) : m_a_851( a_851 ) // UndCC_Violation
	{
	}
	int GetmB_851 ( void )
	{
		return ( m_b_851 );
	}
private:
	int m_a_851;
	int m_b_851;
	static int m_c_851;
};
C_851 c_851( 5 );


int main_851 ( void )
{
	if ( c_851.GetmB_851( ) > 0 ) // m_b has not been initialized
	{
	}
	
	int x_851;
	
	if ( x_851 )
	{
	}
}
