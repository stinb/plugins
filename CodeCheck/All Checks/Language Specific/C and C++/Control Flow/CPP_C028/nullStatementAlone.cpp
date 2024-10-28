
void nullTest(){
  while ( ( port & 0x80 ) == 0 ) 
  { 
   int i; /* Not Empty */
   ; /* wait for pin - Compliant */ 
    ;   
   int j; /* Not Empty */
   ;/* wait for pin - Not compliant, no white-space char after ; */  // UndCC_Violation
   ; int l; /* Not Compliant, code following */ // UndCC_Violation
   }
   for(;;){ //Exception // UndCC_Violation
     i++;
   }
}
