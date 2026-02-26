// Custom

class A {
public:
  int f1()         // UndCC_Violation - can be const
  {
    return m_i;
  }

  int f2()         // UndCC_Violation - can be static
  {
    return m_s;
  }

  int f3()         // UndCC_Valid - cannot be const or static
  {
    return ++m_i;
  }

  static int f4()  // UndCC_Valid
  {
	  return 1;
  }

  int f5() const { // UndCC_Valid - cannot be static
	  return m_i;
  }

private:
  int m_i;
  static int m_s;
};
