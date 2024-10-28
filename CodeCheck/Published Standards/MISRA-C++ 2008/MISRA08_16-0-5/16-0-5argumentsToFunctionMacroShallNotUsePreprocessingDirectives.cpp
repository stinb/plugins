#include <cstdio>

#define M(A) printf(#A)
int main()
{
  /* UndCC_Violation */ M (
#ifdef SW
      "Message 1"
#else
      "Message 2"
#endif
  );
}
