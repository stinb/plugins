// Custom

#include <csetjmp>

static jmp_buf env;

int main() {
#if defined(_WIN32) || defined(__APPLE__)
  void (*myLeastFavoriteFunction)(jmp_buf, int) = &longjmp; // UndCC_Valid
#endif
  setjmp(env);          // UndCC_Violation
  _setjmp(env);         // UndCC_Violation
  longjmp(env, 1);      // UndCC_Violation

}
