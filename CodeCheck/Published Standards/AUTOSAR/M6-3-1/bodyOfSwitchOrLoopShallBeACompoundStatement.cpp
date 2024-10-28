extern int N_ELEMENTS;
extern int buffer [];
extern void process_data();
extern void service_watchdog();
extern int new_data_available;

int main () {
  int i;
  for ( i = 0; i < N_ELEMENTS; ++i )
  {                   // UndCC_Valid
	  buffer [ i ] = 0; // Even a single statement must
	  // be in braces
  }

  for ( i = 0; i < N_ELEMENTS; ++i ); // UndCC_Violation
                                      // Accidental single null statement
  {
    buffer [ i ] = 0;
  }

  while ( new_data_available ) // UndCC_Violation
    process_data ( );          // Incorrectly not enclosed in braces
    service_watchdog ( );      // Added later but, despite the appearance
                               // (from the indent) it is actually not
                               // part of the body of the while statement,
                               // and is executed only after the loop has
                               // terminated
  return 0;
}
