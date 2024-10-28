#include <stdio.h>
#include <stdlib.h>

int main ()
{
    FILE * pFile;
    pFile= fopen ("myfile.txt","r");
    if (pFile == NULL)
    {
            fputs ("error opening file\n",stderr);
            abort();  // UndCC_Violation - if abort is removed, next violation in file (line 20) will be flagged
    }
    fclose (pFile);

    FILE * testFile;
    pFile = fopen ("testfile.txt","r");
    if (testFile==NULL)
    {
            printf ("Error opening file");
            exit (1);
    }
    else
    {
            /* other stuff here */
    }

    char * pPath;
    pPath = getenv ("PATH");
    if (pPath!=NULL)
            printf ("The current path is: %s",pPath);

    int i;
    printf ("Checking if processor is available...");
    if (system(NULL)) puts ("Ok");
    else exit (1);
    printf ("Executing command DIR...\n");
    i=system ("dir");
    printf ("The value returned was: %d.\n",i);

    return 0;
}
