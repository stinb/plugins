#define UINT_32 unsigned int
int get_status();
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
}
int get_status() {
  return 1;
}
