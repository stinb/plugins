class A_933
{
public:
  int f1_933 ( ) // UndCC_Violation � can be const
  {
    return m_i_933;
  }
  int f2_933 ( ) // UndCC_Violation � can be static
  {
    return m_s_933;
  }
  int f3_933 ( ) // UndCC_Valid � cannot be const or static
  {
    return ++m_i_933;
  }
  static int f4_993 ()
  {
	  return 1;
  }
  int f5_993() const {
	  return m_i_933;
  }
private:
  int m_i_933;
  static int m_s_933;
};
