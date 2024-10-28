// MISRA C 2012

void compliant( int flag_1, int flag_2 )
{
  if ( flag_1 )
  {
    action_1 ( );
  }
  else if ( flag_2 )
  {
    action_2 ( );
  } // UndCC_Violation
}

void non_compliant( int flag_1, int flag_2 )
{
  if ( flag_1 )
  {
    action_1 ( );
  }
  else if ( flag_2 )
  {
    action_2 ( );
  }
  else
  {
    ; /* No action required - ; is optional */
  }
}

