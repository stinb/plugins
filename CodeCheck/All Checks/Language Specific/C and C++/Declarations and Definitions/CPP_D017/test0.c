// Custom

// a declaration mentioning inline
inline int max(int a, int b); // UndCC_Violation

// a definition mentioning inline
inline int max(int a, int b) { // UndCC_Violation
  return a > b ? a : b;
}

// a declaration mentioning inline
static inline int min(int a, int b);

// a definition mentioning inline
inline static int min(int a, int b) {
  return a < b ? a : b;
}

// a declaration mentioning inline
inline static int eq(int a, int b);

// a definition mentioning inline
inline int eq(int a, int b) {
  return a == b ? 1 : 0;
}

// a definition mentioning inline
static inline int ne(int a, int b) {
  return a != b ? 1 : 0;
}

// a declaration mentioning inline
inline int ne(int a, int b);
