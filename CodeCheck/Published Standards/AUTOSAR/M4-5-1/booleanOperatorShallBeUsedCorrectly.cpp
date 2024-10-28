bool b1 = true;
bool b2 = false;
int s8a;
int main() {
  if ( b1 & b2 ) // UndCC_Violation
  if ( b1 < b2 ) // UndCC_Violation
  if ( ~b1 ) // UndCC_Violation
  if ( b1 ^ b2 ) // UndCC_Violation
  if ( b1 == false ) // UndCC_Valid
  if ( b1 == b2 ) // UndCC_Valid
  if ( b1 != b2 ) // UndCC_Valid
  if ( b1 && b2 ) // UndCC_Valid
  if ( !b1 ) // UndCC_Valid
  s8a = b1 ? 3 : 7; // UndCC_Valid
  if ( &b2 ) {}
}
