float My_fabs ( float f )
{
  unsigned char * pB = reinterpret_cast< unsigned char * >( &f ); // UndCC_Violation
  *( pB + 3 ) &= 0x7f;  
                       // of an IEEE-754 float value.
  return ( f );
}


union fp_bit_twiddler {
    float f;
    int i; // UndCC_Violation
} q;

int main () {
  q.f = 3.14159;
  q.i &= (1 << 3);
  float a = q.f;
  float pi = 3.14159;
  (unsigned char *) &pi;
  return 0;
}


