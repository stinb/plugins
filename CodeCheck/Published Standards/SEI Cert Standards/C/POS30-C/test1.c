typedef signed int ssize_t;

void foo()
{
    char buf[1024];
    ssize_t buffer_size = sizeof(buf);
    ssize_t len = readlink("/usr/bin/perl", buf, buffer_size);  //UndCC_Violation
    buf[len] = '\0';
}

