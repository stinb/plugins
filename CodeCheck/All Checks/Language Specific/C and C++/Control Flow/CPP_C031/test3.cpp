// Custom

void okay1()
{
  switch (0) {
    default : ;
  }                 // UndCC_Valid
}

void okay2()
{
  switch (0) {
    case 0 : break;
    default : ;
  }                 // UndCC_Valid
}

void okay3()
{
  switch (0) {
    case 0 :
    case 1 :
    default : ;
  }                 // UndCC_Valid
}
void okay4()
{
  switch (0) {
    case 0 :
      if (0)
        break;
      else
        return;
    default : ;
  }                 // UndCC_Valid
}
void okay5()
{
  switch (0) {
    case 0 :
      if (0)
        1;
      break;
    default: ;
  }                 // UndCC_Valid
}
void okay6()
{
  switch (0) {
    case 0 : {
      if (0)
        1;
      }
      break;
    default : ;
  }                 // UndCC_Valid
}
void okay7()
{
  switch (0) {
#ifdef FOO
    case 0 :
      {
        if (0)
          1;
      }
      break;
#endif
    default : ;
  }                 // UndCC_Valid
}


void violation1()
{
  switch (0) {}     // UndCC_Violation - no default
}

void violation2()
{
  switch (0) {
    default : break;
    case 0 : ;
  }                 // UndCC_Violation - case after default
}

void violation3()
{
  switch (0) {
    default :
      switch (1) {} // UndCC_Violation - no default
  }                 // UndCC_Valid
}

void violation4()
{
  switch (0) {
    case 0 :
      switch (1) {
        default : ;
      }             // UndCC_Valid
  }                 // UndCC_Violation - no default
}
