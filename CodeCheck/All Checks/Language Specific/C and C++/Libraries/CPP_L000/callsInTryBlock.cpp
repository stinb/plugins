void no_throw() throw ();
void does_throw() throw (int);
void may_throw();
void no_check();
void do_check();


void okay1()
{
  no_throw();   // UndCC_Valid because it claims no throw
  no_check();   // UndCC_FalsePos(1) with default option
  try {
    does_throw();
    may_throw();
    do_check();
    try {
      does_throw();
      { does_throw(); }
    } catch (int) {};
  } catch (int) {};
}

void violation1()
{
  does_throw(); // UndCC_Violation, because it claims throw
  may_throw();  //  UndCC_Violation(1) unknown, makes no throw claim
  do_check();   // UndCC_Violation(1), sample we are forcing; see CUSTOMIZE
  try {} catch (int) {};
  does_throw(); // UndCC_Violation
}
