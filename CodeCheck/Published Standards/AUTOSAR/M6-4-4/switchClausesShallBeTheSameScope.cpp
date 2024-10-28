extern int x_644;
extern int y_644;
extern void DoIt_644();

int main () {
  switch ( y_644 )
  {
  case 1:
	break;
  case 2:
	  switch ( x_644 )
	  {
		  case 1: // UndCC_Valid
			  if ( true )
			  {
			  case 2: // UndCC_Violation
				  DoIt_644 ( );
			  }
			  break;
		  default:
			  break;
	  }
  default:
	break;
  }
  return 0;
}
