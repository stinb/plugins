typedef signed int ssize_t;

void foo()
{
    char buf[1024];
    ssize_t len = readlink("/usr/bin/perl", buf, sizeof(buf));  //UndCC_Violation
    buf[len] = '\0';
}

