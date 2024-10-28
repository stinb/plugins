enum { COLOUR_0, COLOUR_1, COLOUR_2, COLOUR_COUNT } colour;
int main () {
  if ( COLOUR_0 == colour ) // UndCC_Valid
  if ( ( COLOUR_0 + COLOUR_1 ) == colour ) // UndCC_Violation
  if ( colour < COLOUR_COUNT ) // UndCC_Valid
  return 0;
}

