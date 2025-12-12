#define UINT_32 unsigned int
#include <stdint.h>
int get_status();

uint8_t g_c[10] = {1, 5, 3, 9, 2, 8, 4, 6, 7, 0};
uint8_t g_array[10][10];

int main() {
  UINT_32 uLen = 123456789;
  UINT_32 m_count[2] = {0};
  m_count[1] += (uLen >> 29);
  int j = 0;
  int flag;
  int sum = 0;

  /*  j is incremented as part of init */
  for (int i = j++; i < 10; ++i) { } // UndCC_Violation
  int x = 0;
  int *p = &x;
  for (int i = (*p = 1); i < 10; ++i) { } // UndCC_Violation
  for (int i = 0, j = 0; i < 10; ++i) { /* ... */ } // UndCC_Violation
  for (int i = 0; ; i++) { } // UndCC_Violation
  int N;
  for (int i = 0; i < N; i++) {   /* UndCC_Violation: N is modified inside */
    N = N + 1;        /* Loop bound changes during iteration */
  }
  for (int i = 0; (i < 10) && (++flag > 0); i++) { }/* UndCC_Violation: ++flag introduces a side effect in the condition */
  for (int i = 0; x != 0; i++) { }   /* UndCC_Violation: condition ignores loop counter i */
  for (int i = 0; i < 10; ) { }  /* UndCC_Violation: iteration clause missing */
  for (int i = 0, j = 0; i < 10; i++, j++) { } /* UndCC_Violation: iteration clause modifies both i and j */
  for (int i = 0; i < 10; i++, flag = get_status()) { } /* UndCC_Violation: function call with side effects in iteration clause */
  for (int i = 0; i < 10; sum += i, i++) { } /* UndCC_Violation: iteration clause modifies sum (unrelated to loop control) */
  for (;;) {} // UndCC_Valid

  /* UndCC_Valid: k is not the loop counter, modifying it in the body is allowed */
  uint8_t k = 0;
  for (uint8_t i = 1; i < 8; i++) { // UndCC_Valid
    if (g_c[i] > g_c[k]) {
      k = i;
    }
  }
}

void rgBusWr(unsigned int addr, unsigned char val, int param) {
  // Dummy function
}

void g(void) {
  unsigned char i;
  for (i = 0; i < 8; i++) { // UndCC_Valid - i is only read in the body, not modified
    rgBusWr(0x10a6 + i, 0x55, 1);
  }
}

void h(void) {
  uint8_t cycle = 10;
  for (uint8_t i = 0; i < cycle; i++) { // UndCC_Valid - modifying array element, not loop counter i
    g_array[i][5] = 42;
  }
}

int get_status() {
  return 1;
}
