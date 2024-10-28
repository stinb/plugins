// Custom

void fn()
{
  char c = 'c';  // UndCC_Valid - Exception for =
  int i = 1;     // UndCC_Valid - Exception for =
  float f = 2.3; // UndCC_Valid - Exception for =

  struct Initials { char first; char last; };
  struct Initials initials = { .first = 'r', .last = 'b' };
  struct Initials *initialsPtr = &initials;

  // Precedence 1
  c++;                   // UndCC_Violation
  initials.first;
  (initials.last)--;     // UndCC_Violation
  initialsPtr->first;
  (initialsPtr->last++); // UndCC_Violation

  // Precedence 2
  ++i;
  ++c;          // UndCC_Violation
  --f;
  --c;          // UndCC_Violation
  +i;
  +c;           // UndCC_Violation
  -f;
  -c;           // UndCC_Violation
  -2.3;
  -'c';         // UndCC_Violation
  !i;
  !c;           // UndCC_Violation
  !7;
  !'c';         // UndCC_Violation
  ~i;
  ~c;           // UndCC_Violation
  ~3;
  ~'c';         // UndCC_Violation
  &i;
  &c;           // UndCC_Valid - Exception for unary & operator
  (int) c;
  (int) 'c';
  (char) i;
  sizeof c;
  sizeof 'c';
  sizeof i;
  _Alignof c;
  _Alignof 'c';
  _Alignof i;

  // Precedence 3
  i * 3.2;
  c * 'c';       // UndCC_Violation
  f / i;
  c / ((c));     // UndCC_Violation
  7 % 3;
  ('c') % 'c';   // UndCC_Violation

  // Precedence 4
  i + 1;
  c + 1;   // UndCC_Violation
  7 - 1;
  'c' - 1; // UndCC_Violation
  '0' + 1; // UndCC_Valid - Exception for '0' + integer
  1 + '0'; // UndCC_Valid - Exception for '0' + integer

  // Precedence 5
  i << 1;
  c << 1;     // UndCC_Violation
  i >> 2;
  c >> '1';   // UndCC_Violation
  7 >> 1;
  'c' >> '1'; // UndCC_Violation

  // Precedence 6
  i < 2;
  'c' < 2; // UndCC_Violation
  f > 2;
  'c' > 2; // UndCC_Violation
  i <= 2;
  'c' <= 2; // UndCC_Violation
  f >= 2;
  'c' >= 2; // UndCC_Violation
  i < 2;
  '0' < 1.3;
  '0' < (1 + 3); // UndCC_Valid - Exception for '0'...'9' with < <= > >=
  2 >= '0';      // UndCC_Valid - Exception for '0'...'9' with < <= > >=

  // Precedence 7
  i == i;
  c == c;
  1 == 1;
  'c' == 'c';
  i != i;
  c != c;
  1 != 1;
  'c' != 'c';

  // Precedence 8
  i & i;
  c & c;     // UndCC_Violation
  2 & 2;
  'c' & 'c'; // UndCC_Violation

  // Precedence 9
  i ^ i;
  c ^ c;     // UndCC_Violation
  3 ^ 3;
  'c' ^ 'c'; // UndCC_Violation

  // Precedence 10
  i | i;
  c | c;     // UndCC_Violation
  4 | 4;
  'c' | 'c'; // UndCC_Violation

  // Precedence 11
  i && i;
  c && c;     // UndCC_Violation
  5 && 5;
  'c' && 'c'; // UndCC_Violation

  // Precedence 12
  i || i;
  c || c;     // UndCC_Violation
  6 || 6;
  'c' || 'c'; // UndCC_Violation

  // Precedence 13
  'c' ? c : 0;
  'c' ? c : 0;
  'c' ?: 0;
  'c' ?: 0;

  // Precedence 14
  c = 'c';
  i += i;
  c += i;   // UndCC_Violation
  i += 1;
  i += 'c'; // UndCC_Violation
  i -= i;
  c -= i;   // UndCC_Violation
  i -= 1;
  i -= 'c'; // UndCC_Violation
  i *= i;
  c *= i;   // UndCC_Violation
  i *= 1;
  i *= 'c'; // UndCC_Violation
  i /= i;
  c /= i;   // UndCC_Violation
  i /= 1;
  i /= 'c'; // UndCC_Violation
  i %= i;
  c %= i;   // UndCC_Violation
  i %= 1;
  i %= 'c'; // UndCC_Violation
  i >>= i;
  c >>= i;   // UndCC_Violation
  i >>= 1;
  i >>= 'c'; // UndCC_Violation
  i <<= i;
  c <<= i;   // UndCC_Violation
  i <<= 1;
  i <<= 'c'; // UndCC_Violation
  i &= i;
  c &= i;   // UndCC_Violation
  i &= 1;
  i &= 'c'; // UndCC_Violation
  i ^= i;
  c ^= i;   // UndCC_Violation
  i ^= 1;
  i ^= 'c'; // UndCC_Violation
  i |= i;
  c |= i;   // UndCC_Violation
  i |= 1;
  i |= 'c'; // UndCC_Violation

  // Precedence 15
  c = i, i;
  c = c, c;
  c = 1, 2;
  c = 'a', 'b';
  i = i, i;
  i = c, c;
  i = 1, 2;
  i = 'a', 'b';
}
