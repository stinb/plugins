// Custom

void foo ( )
{
  throw ( 21 );
}

void goo ( ) throw ( long )
{
  foo ( ); // UndCC_Violation - goo throws int here
}
