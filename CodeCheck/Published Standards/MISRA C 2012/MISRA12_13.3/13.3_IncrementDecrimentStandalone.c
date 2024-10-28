// This is a test file to demonstrate the increment and decrement
// codecheck written by Kacy Stocks 8/24/2018.

void myFunc() {

int i;
int j;
int k;

//below is incorrect syntax
// 1:
j = i++; // UndCC_Violation
// 2:
i = k - i--; // UndCC_Violation
// 3:
j += k++; // UndCC_Violation

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
j+= k;

// notice: Because for loops have multiple statement endings, for loops are 'ok' syntactically.
for (i=0; i<10; i++){ // because statements end at the ';' and '{'
  k += i;
}

}
