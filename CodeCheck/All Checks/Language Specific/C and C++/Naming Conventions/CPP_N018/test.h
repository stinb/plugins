//This example is in an ifndef, and is checking inactive code. 

#ifndef FOO_H 
#define FOO_1 //UndCC_Valid if checking for numbers within the name
#define Foo //UndCC_Violation has lowercase letters
#define Foo_2 //UndCC_Violation has lowercase letters and numbers
#define FOO //UndCC_Valid is all capitalized


int main() {

   return 0;
}
#endif
