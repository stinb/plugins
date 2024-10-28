#include "cstdio"

#define M(A) printf(#A)
int main()
{
    M(
#ifdef SW // UndCC_Violation
        "Message 1"
#else  // UndCC_Violation
        "Message 2"
#endif // UndCC_Violation
    );
    return 0;
}
