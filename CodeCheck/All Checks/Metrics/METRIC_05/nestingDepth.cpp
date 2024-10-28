// Test: check if function depth exceeds 5.

void Okay1() {}
void Okay2()
{
  if (1)
    if (2)
      if (3)
        if (4)
          if (5)
            ;
}


void Violation1() // UndCC_Violation
{
  if (1)
    if (2)
      if (3)
        if (4)
          if (5)
            if (6)
              ;
}
