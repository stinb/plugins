/* offsetof example */
#include <stdio.h>
#include <stddef.h>

struct mystruct {
	char singlechar;
	char arraymember[10];
	char anotherchar;
};

int main ()
{
	printf ("offsetof(mystruct,singlechar) is %d\n",offsetof(struct mystruct,singlechar)); // UndCC_Violation
	printf ("offsetof(mystruct,arraymember) is %d\n",offsetof(struct mystruct,arraymember)); // UndCC_Violation
	printf ("offsetof(mystruct,anotherchar) is %d\n",offsetof(struct mystruct,anotherchar)); // UndCC_Violation

	return 0;
}
