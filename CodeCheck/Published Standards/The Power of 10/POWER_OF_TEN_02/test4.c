// Endless loops with possible loop terminations

enum { LIMIT = 8 };

void endless_loops_with_for(void)
{
  while (1) { // UndCC_Valid - endless loop without loop termination
    for (int i = 0; i < 10; i++) { // UndCC_Valid
      break;
    }
  }

  while (1) { // UndCC_Violation - endless loop with loop termination
    for (int i = 0; i < 10; i++) { // UndCC_Valid
      return;
    }
  }

  while (1) { // UndCC_Violation - endless loop with loop termination
    for (int i = 0; i < 10; i++) { // UndCC_Valid
      goto END_0;
    }
  }
END_0:
  return;
}

void endless_loops_with_switch(void)
{
  int x = 0;
  while (1) { // UndCC_Valid - endless loop without loop termination
    switch (x) {
      case LIMIT:
        break;
    }
  }

  while (1) { // UndCC_Violation - endless loop with loop termination
    switch (x) {
      case LIMIT:
        return;
    }
  }

  while (1) { // UndCC_Violation - endless loop with loop termination
    switch (x) {
      case LIMIT:
        goto END_1;
    }
  }
END_1:
  return;
}
