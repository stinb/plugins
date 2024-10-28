#include <stdio.h>

enum
{
    BUFFERSIZE = 32
};

extern void initialize_data(char *data, size_t size);

void func(const char *file_name)
{
    char data[BUFFERSIZE];
    char append_data[BUFFERSIZE];
    FILE *file;

    file = fopen(file_name, "a+");
    if (file == NULL)
    {
        /* Handle error */
    }

    initialize_data(append_data, BUFFERSIZE);
    if (fwrite(append_data, BUFFERSIZE, 1, file) != BUFFERSIZE)
    {
        /* Handle error */
    }

    if (fseek(file, 0L, SEEK_SET) != 0)
    {
        /* Handle error */
    }

    if (fread(data, BUFFERSIZE, 1, file) != 0)
    {
        /* Handle there not being data */
    }

    if (fclose(file) == EOF)
    {
        /* Handle error */
    }
}