#include <cstdio>

int main (){
    //this volatile should not be included
    volatile int value; // UndCC_Violation
    value++;
}