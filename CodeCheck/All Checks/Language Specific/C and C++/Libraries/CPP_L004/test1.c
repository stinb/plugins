#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *pFile;
	pFile = fopen("myfile.txt", "r");
	if (pFile == NULL)
	{
		fputs("error opening file\n", stderr);
		abort(); // UndCC_Violation(1)
	}
	fclose(pFile);

	FILE *testFile;
	pFile = fopen("testfile.txt", "r");
	if (testFile == NULL)
	{
		printf("Error opening file");
		exit(1); // UndCC_Violation(1)
	}
	else
	{
		/* other stuff here */
	}

	char *pPath;
	pPath = getenv("PATH"); // UndCC_Violation(1)
	if (pPath != NULL)
		printf("The current path is: %s", pPath);

	int i;
	printf("Checking if processor is available...");
	if (system(NULL)) // UndCC_Violation(0, 1)
		puts("Ok");
	else
		exit(1); // UndCC_Violation(1)
	printf("Executing command DIR...\n");
	i = system("dir"); // UndCC_Violation(0,1)
	printf("The value returned was: %d.\n", i);

	return 0;
}
