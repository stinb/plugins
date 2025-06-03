#include <stdlib.h>
#include <string.h>

#define CMD_MAX 256

int main(int argc, char** argv) {
    char cmd[CMD_MAX] = "/usr/bin/cat ";
    strcat(cmd, argv[1]);
    system(cmd); // UndCC_Violation
    return 0;
}
