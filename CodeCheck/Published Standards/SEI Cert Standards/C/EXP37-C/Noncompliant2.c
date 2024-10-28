#include <stdio.h>
#include <string.h>

char *(*fp)();

int main(void)
{
    const char *c;
    fp = strchr;
    c = fp('e', "Hello"); // UndCC_FalseNeg, not covered due to limitation, cannot get relevant info on strchr
    printf("%s\n", c);
    return 0;
}
