// This is a test file to demonstrate the increment and decrement
// codecheck written by Kacy Stocks 8/24/2018.

int i;
int j;
int k;

int main()
{
  // below is incorrect syntax
  //  1:
  j = i++; // UndCC_Violation
  // 2:
  i = k - i--; // UndCC_Violation
  // 3:
  j += k++; // UndCC_Violation

  // The following are violation to the rule:
  // 4:
  // k-- = i;
  // 5:
  // i++ = j--;

  // these can sometimes be conceptually unclear and cause unintended
  // interpretations on systems with differences in compilers.

  // below is correct syntax

  // #1 could alternately be written like so:
  i++;
  j = i;
  // which is much easier to understand

  // #2:
  i--;
  i = k - i;

  // #3:
  k++;
  j += k;

  // #4:
  k--;
  k = i;

  // #5:
  i++;
  j--;
  i = j;

  // notice: Because for loops have multiple statement endings, for loops are 'ok' syntactically.
  for (i = 0; i < 10; i++)
  { // because statements end at the ';' and '{'
    k += i;
  }
}