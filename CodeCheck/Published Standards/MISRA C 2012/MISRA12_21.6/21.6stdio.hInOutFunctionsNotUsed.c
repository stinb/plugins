#include <stdio.h>


int main() {
    printf ("Integers: %i %u \n", -3456, 3456); // UndCC_Violation
    printf ("Characters: %c %c \n", 'z', 80);
    printf ("Decimals: %d %ld\n", 1997, 32000L);
    printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    printf ("floats: %4.2f %+.0e %E \n", 3.14159, 3.14159, 3.14159);
    printf ("Preceding with empty spaces: %10d \n", 1997);
    printf ("Preceding with zeros: %010d \n", 1997);
    printf ("Width: %*d \n", 15, 140);
    printf ("%s \n", "Educative");
    return 0;
}
