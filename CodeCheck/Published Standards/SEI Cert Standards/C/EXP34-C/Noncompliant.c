#if __has_include(<png.h>)

#include <png.h> /* From libpng */
#include <string.h>

void func(png_structp png_ptr, int length, const void *user_data)
{
    png_charp chunkdata;
    chunkdata = (png_charp)png_malloc(png_ptr, length + 1);
    /* ... */
    memcpy(chunkdata, user_data, length); // UndCC_Violation(lin)
    /* ... */
}

#endif // __has_include(<png.h>)
