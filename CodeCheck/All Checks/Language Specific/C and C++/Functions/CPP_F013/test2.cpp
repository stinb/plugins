// Custom

class D
{
public:
  D ( int * a ) : m_shared ( a )
  {
    int *b = a;
    m_shared = (b);
  }
  int * getA ()
  {
    return m_shared; // UndCC_Valid - m_shared is not class-data
  }
private:
  int * m_shared;
};
