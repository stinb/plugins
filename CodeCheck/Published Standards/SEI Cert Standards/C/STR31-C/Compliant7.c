#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    /* Ensure argv[0] is not null */
    const char *const name = (argc && argv[0]) ? argv[0] : "";
    char *prog_name = (char *)malloc(strlen(name) + 1);
    if (prog_name != NULL)
    {
        strcpy(prog_name, name);
    }
    else
    {
        /* Handle error */
    }
    free(prog_name);
    return 0;
}