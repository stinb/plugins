// Custom

int validGlobal = 0,
  invalidGlobal = 0; // UndCC_Violation

void increaseGlobalCount()
{
  ++validGlobal;
  ++invalidGlobal;
}

int printGlobalCount()
{
  int validLocal;
  return
    printf("increaseGlobalCount invoked %d %s\n",
      validGlobal,
      (validGlobal == 1 ? "time" : "times")
    );
}
