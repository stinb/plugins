// Test: check if function depth exceeds 5.

void Okay1() {}
void Okay2()
{
  if (1)
    if (2)
      if (3)
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
              if(7)
                if(8)
                  if(9)
                    if(10)
                      if(11)
                        if(12)
                          if(13)
                            if(14)
                              if(15)
                                if(16)
                                  if(17)
                                    if(18)
                                      if(19)
                                        if(20)
                                          if(21)
                                            ;
}
