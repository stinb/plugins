#include <stdio.h>

int close_stdout(void)
{
    if (fclose(stdout) == EOF)
    {
        return -1;
    }

    printf("stdout successfully closed.\n"); // UNDCC_Violation, stdout is accessed after closing
    return 0;
}

int close_stderr(void)
{
    if (fclose(stderr) == EOF)
    {
        return -1;
    }

    perror("stderr successfully closed.\n"); // UNDCC_Violation, stderr is accessed after closing
    return 0;
}
