int array1[ 10 ];              /* UndCC_Valid */
extern int array2[ ];          /* UndCC_Violation */
int array2[ ] = { 0, 10, 15 }; /* UndCC_Valid */
