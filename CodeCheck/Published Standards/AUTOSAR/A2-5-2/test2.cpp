// Custom

void a()
{
}
void b()
<%                       // UndCC_Violation
%>                       // UndCC_Violation

int c[10];
int d
<:                       // UndCC_Violation
  10
:>;                      // UndCC_Violation

#define e
%:define f               // UndCC_Violation

#define g(a, b) a ## b
#define h(a, b) a %:%: b // UndCC_Violation
