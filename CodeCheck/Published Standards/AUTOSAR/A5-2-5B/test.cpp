// Custom
#include <cstdint>

int main()
{

    int32_t arr1[10];
    arr1[0] = 2; /* UndCC_Valid */

    int32_t arr2[10];
    arr2[11] = 2; /* UndCC_Violation */
}
