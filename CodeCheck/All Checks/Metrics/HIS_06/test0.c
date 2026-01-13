// Custom

void validCall1()
{
}

void validCall2()
{
}

void validCall3()
{
}

void validCall4()
{
}

void validCall5()
{
}

void validCall6()
{
  validCall1();
}

void validCall7()
{
  validCall1();
}

void validCall8()
{
  validCall1();
}

void invalidCalls() // UndCC_Violation
{
  validCall1();
  validCall2();
  validCall3();
  validCall4();
  validCall5();
  validCall6();
  validCall7();
  validCall8();
}
