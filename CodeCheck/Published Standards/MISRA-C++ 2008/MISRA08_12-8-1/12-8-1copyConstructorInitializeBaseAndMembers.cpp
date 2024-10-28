class A_1281
{
public:
	A_1281 ()
	{
	}
	

	
	A_1281 ( const A_1281 & rhs )
		: m_i_1281 ( rhs.m_i_1281 )
	{
		++m_static_1281; // UndCC_Violation
	}
	
private:
	int m_i_1281;
	static int m_static_1281;
};

int A_1281::m_static_1281 = 0;
A_1281 f_1281 ( )
{
	return A_1281 ( );
}

void b_1281 ( )
{
	A_1281 a_1281 = f_1281 ( );
}
