#include <assert.h>
#include <string.h>

struct foo_header
{
    int len;
    /* ... */
};

void func(void)
{
    char c = 'x';
    int *ip = (int *)&c; // UndCC_Violation /* This can lose information */
    char *cp = (char *)ip;

    /* Will fail on some conforming implementations */
    assert(cp == &c);
}

void foo_2(char *data, size_t offset)
{
  struct foo_header *tmp;
  struct foo_header header;

  tmp = (struct foo_header *)(data + offset);
  memcpy(&header, tmp, sizeof(header));       // UndCC_Violation(Win,Lin)

  /* ... */
}
