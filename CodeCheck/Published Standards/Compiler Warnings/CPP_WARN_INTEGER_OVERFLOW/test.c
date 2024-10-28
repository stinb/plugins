#include <limits.h>      

int main(void) {
    int i = INT_MAX + 1; //UndCC_Violation 
    int j= INT_MAX - 2; //UndCC_Valid
    return 0;
}

