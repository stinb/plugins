// Custom

#define BUFFER_SIZE 4096                // UndCC_Valid

void process(void)
{
  char buffer[BUFFER_SIZE];             // UndCC_Valid
  for (int i = 0; i < BUFFER_SIZE; i++) // UndCC_Valid
    ;
  char *cursor = buffer;                // UndCC_Valid
  if (cursor >= buffer + BUFFER_SIZE)   // UndCC_Valid
    ;
}
