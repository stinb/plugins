// 3-1-1possibleToIncludeMultipleTranslationsWithoutViolatingOneDefinition.h
void f1 ( ); // UndCC_Valid
void f2 ( ) { } // UndCC_Violation
inline void f3 ( ) { } // UndCC_Valid
template <typename T>
void f4 ( T ) { } // UndCC_Valid
int a; // UndCC_Violation
