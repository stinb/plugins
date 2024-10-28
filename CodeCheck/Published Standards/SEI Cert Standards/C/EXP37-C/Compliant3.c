#include <stdio.h>
#include <string.h>

char *(*fp)(const char *, int);

int main(void)
{
    const char *c;
    fp = strchr;
    c = fp("Hello", 'e');
    printf("%s\n", c);
    return 0;
}
