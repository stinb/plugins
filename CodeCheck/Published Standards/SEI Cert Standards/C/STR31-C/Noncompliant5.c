#include <string.h>

int main(int argc, char *argv[])
{
    /* Ensure argv[0] is not null */
    const char *const name = (argc && argv[0]) ? argv[0] : "";
    char prog_name[128];
    strcpy(prog_name, name);

    return 0;
}