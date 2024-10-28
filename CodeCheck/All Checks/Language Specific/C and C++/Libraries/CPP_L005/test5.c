#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void f() {
	double n, m;
	double pi = 3.1415926535;
	char floatInput[256];
	printf("Enter degrees: ");
	fgets(floatInput, 256, stdin);
	n = atof(floatInput); // UndCC_Violation
	m = sin(n * pi / 180);
	printf("The sine of %f degrees is %f\n" , n, m);

	int i;
	char intInput[256];
	printf("Enter a number: ");
	fgets(intInput, 256, stdin);
	i = atoi(intInput); // UndCC_Violation
	printf("The value entered is %d. The double is %d.\n", i, i * 2);

	long int li;
	char longInput[256];
	printf("Enter a long number: ");
	fgets(longInput, 256, stdin);
	li = atol(longInput); // UndCC_Violation
	printf("The value entered is %ld. The double is %ld.\n", li, li * 2);
}
