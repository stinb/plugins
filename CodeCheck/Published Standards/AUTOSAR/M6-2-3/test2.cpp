// Custom

void nullStmtComment2(int port)
{
  while ( ( port & 0x80 ) == 0 )
  {
   int i; /* Not Empty */
   ; /* wait for pin - UndCC_Valid */
    ;
   int j; /* Not Empty */
   ;
   ; int l;  // UndCC_Violation
  }
  int i;
  for(;;){ //Exception
    i++;
  }
}
