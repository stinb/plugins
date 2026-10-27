typedef long intptr_t;
int logCall(int failed, intptr_t address, const char *name);

void logsItself(int argument) // UndCC_Valid - taking its own address is not a call
{
  (void)logCall(argument <= 0, (intptr_t)logsItself, "logsItself");
}

void logsAndRecurses(int argument) // UndCC_Violation - it also calls itself
{
  (void)logCall(argument <= 0, (intptr_t)logsAndRecurses, "logsAndRecurses");
  if (argument > 0) {
    logsAndRecurses(argument - 1);
  }
}

void pointerTarget(void);
void (*pending)(void);

void takesPointer(void) // UndCC_Violation - takes the address of a function that calls it
{
  pending = pointerTarget;
}

void pointerTarget(void) // UndCC_Violation
{
  takesPointer();
}
