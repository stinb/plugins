#include <stdio.h>
void fn(void)
{
    FILE *fw = fopen("tmp", "r+"); /* "r+" opens for read/write */
    FILE *fr = fopen("tmp", "r");  /* UndCC_Violation */
}

void read_only_binary(void)
{
    FILE *a = fopen("bin", "rb");
    FILE *b = fopen("bin", "rb"); /* UndCC_Valid - both read-only */
}

void read_only_mixed(void)
{
    FILE *a = fopen("mix", "r");
    FILE *b = fopen("mix", "rb"); /* UndCC_Valid - both read-only */
}

void write_only(void)
{
    FILE *a = fopen("log", "w");
    FILE *b = fopen("log", "w"); /* UndCC_Valid - neither stream reads */
}

void append_only(void)
{
    FILE *a = fopen("log2", "a");
    FILE *b = fopen("log2", "a"); /* UndCC_Valid - neither stream reads */
}

void write_then_read(void)
{
    FILE *a = fopen("rw", "w");
    FILE *b = fopen("rw", "r"); /* UndCC_Violation */
}

void third_stream(void)
{
    FILE *a = fopen("three", "r");
    FILE *b = fopen("three", "w"); /* UndCC_Violation */
    FILE *c = fopen("three", "r"); /* UndCC_Violation */
}

void closed_first(void)
{
    FILE *a = fopen("seq", "w");
    fclose(a);
    FILE *b = fopen("seq", "r"); /* UndCC_Valid - not open at the same time */
}

void different_files(void)
{
    FILE *a = fopen("one", "r");
    FILE *b = fopen("two", "w"); /* UndCC_Valid */
}

void append_then_read(void)
{
    FILE *a = fopen("ap", "a");
    FILE *b = fopen("ap", "r"); /* UndCC_Violation */
}

void update_then_write(void)
{
    FILE *a = fopen("upd", "w+"); /* "w+" also reads */
    FILE *b = fopen("upd", "w");  /* UndCC_Violation */
}

void reopened(void)
{
    FILE *a = fopen("fr", "r");
    FILE *b = freopen("fr", "w", stdout); /* UndCC_Violation */
}

void reassigned(void)
{
    FILE *a = fopen("re", "r");
    a = fopen("re", "w"); /* UndCC_Valid - same stream reassigned */
}

void closed_one_of_two(void)
{
    FILE *a = fopen("k", "w");
    FILE *b = fopen("k", "w"); /* UndCC_Valid - neither stream reads */
    fclose(a);
    FILE *c = fopen("k", "r"); /* UndCC_Violation - b is still open for write */
}
