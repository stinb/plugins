#if __has_include(<png.h>)

#include <png.h> /* From libpng */
#include <string.h>
#include <stdint.h>

void func(png_structp png_ptr, size_t length, const void *user_data)
{
    png_charp chunkdata;
    if (length == SIZE_MAX)
    {
        /* Handle error */
    }
    if (NULL == user_data)
    {
        /* Handle error */
    }
    chunkdata = (png_charp)png_malloc(png_ptr, length + 1);
    if (NULL == chunkdata)
    {
        /* Handle error */
    }
    /* ... */
    memcpy(chunkdata, user_data, length);
    /* ... */
}

#endif // __has_include(<png.h>)
