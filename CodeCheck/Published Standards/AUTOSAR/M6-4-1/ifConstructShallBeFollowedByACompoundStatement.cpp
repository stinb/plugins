extern int test1_641;
extern int test2_641;
extern int x_641;
extern int y_641;


int main () {
  if ( test1_641 );     // UndCC_Violation - accidental single null statement
  {
      x_641 = 1;
  }
  if ( test1_641 )
  {
      x_641 = 1;        // UndCC_Valid - a single statement must be in braces
  }
  else if ( test2_641 ) // UndCC_Valid - no need for braces between else and if
  {
      x_641 = 0;        // UndCC_Valid � a single statement must be in braces
  }
  else              // UndCC_Violation
    x_641 = 3;          // This was (incorrectly_641) not enclosed in braces
    y_641 = 2;          // This line was added later but, despite the
                    // appearance (from the indent) it is actually_641 not
                    // part of the else, and is ex_641ecuted unconditionally_641
  return 0;
}
