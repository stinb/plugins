class Matrix
{
private:
  int * p = new int[10];
  double * m_mem;
  int m_width;
  int m_height;
public:
  Matrix( int width, int height );
  ~Matrix(); // destructor

  double operator()( int i, int j ) const
  {
    return m_mem[ i * m_width + j ];
  }
  double& operator()( int i, int j ) 
  {
    return m_mem[ i * m_width + j ];
  }

  int width() const
  {
    return m_width;
  }

  int height() const
  {
    return m_height;
  }

  Matrix(Matrix const& other ); // copy constructor
  Matrix& operator=( Matrix const& other ); // assignment operator
};
