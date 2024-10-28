// Custom

void unused_label ( void )
{
  goto label1;
label1: // UndCC_Valid
label2: // UndCC_Violation
  ;
}
