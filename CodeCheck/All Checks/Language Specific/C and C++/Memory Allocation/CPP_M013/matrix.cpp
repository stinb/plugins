#include "matrix.hpp"

Matrix::Matrix( int width, int height ) :
m_mem( new double[width * height] ),
       m_width( width ),
       m_height( height )
{
}

Matrix::~Matrix()
{
  delete [] m_mem;
}
