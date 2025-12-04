#include <stdio.h>
void fn(void)
{
    FILE *fw = fopen("tmp", "r+"); /* "r+" opens for read/write */
    FILE *fr = fopen("tmp", "r");  /* UndCC_Violation */
}