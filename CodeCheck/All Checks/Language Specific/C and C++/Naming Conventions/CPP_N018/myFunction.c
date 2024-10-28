#define MYMACRO 20;
#define ANOTHER_MACRO 20;
#define __MYMACRO__ 20;
#define ANOTHER_MACRO__ 20;

// INVALID
#define BAD_macro 100;            // UndCC_Violation contains lowercase characters
#define ANOTHER_BAD_MACRO_01 100; // UndCC_Valid if the option to check for numerical values is selected, this macro name will flag a violation
#define __mymacro__ 100;          // UndCC_Violation contains lowercase characters

int main() {

   return 0;
}