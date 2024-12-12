extern int a_645;
extern int b_645;
extern int x_645;

int main () {
  switch ( x_645 )
  {
	case 0:
	  break;  // UndCC_Valid
	case 1:   // UndCC_Valid - empty drop through allows a group
	case 2:
    break;  // UndCC_Valid
  case 3:  // UndCC_Valid
	  throw;
  case 4: // UndCC_Violation
	  a_645 = b_645;
              
	case 5:
	  a_645 = b_645;
#ifdef __SOMETHING_ENABLED__
	  Dosomethingelse();
#endif
	  break;
	default: // UndCC_Violation
	  ;       
  }
  switch(x_645) // UndCC_Valid
  {
  case 0:
    if (b_645) {
      return;
    }
    break;
  }
}


