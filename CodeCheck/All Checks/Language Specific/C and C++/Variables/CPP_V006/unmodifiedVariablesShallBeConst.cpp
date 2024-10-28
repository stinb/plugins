
void b_711 ( int * );

int f_711 ( int * p1_711, // UndCC_Violation
			int * const p2_711, // UndCC_Valid
			int * const p3_711,  // UndCC_Valid
			int x_711 ) // UndCC_Violation
{
  *p1_711 = 10;
  *p2_711 = 10;
  b_711( p3_711 );
  int i_711 = 0; // UndCC_Violation
  x_711++;
  int y_711 = 0;
  y_711+= x_711;
  return i_711;
}
