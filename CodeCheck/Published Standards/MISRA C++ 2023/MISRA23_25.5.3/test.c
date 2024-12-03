#include <locale.h>

void f1(void)
{
    const char *res1;
    const char *res2;
    char copy[128];
    
    res1 = setlocale(LC_ALL, 0);
    (void)strcpy(copy, res1);
    res2 = setlocale(LC_MONETARY, "French");
    
    printf("%s\n", res1); /* UndCC_Violation - use after subsequent call */
    printf("%s\n", copy); /* UndCC_Valid - copy made before subsequent call */
    printf("%s\n", res2); /* UndCC_Valid - no subsequent call before use */
}
