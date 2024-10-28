class C_932
{
public:
	int & getA_932 () 	// UndCC_Violation
	{
		return a_932;
	}
private:
	int a_932;
};

void b_932 ( C_932 & c_932 )
{
	int & a_ref_932 = c_932.getA_932 ();
	a_ref_932 = 10; 		// External modification of private C::a
}

class D_932
{
public:
	D_932 ( int * a ) : m_shared_932 ( a )
	{
		int *b = a;
		m_shared_932 = (b);
	}
	int * getA_932 ()
	{
		return m_shared_932; // UndCC_Valid - m_shared is not class-data
	}
private:
	int * m_shared_932;
};
