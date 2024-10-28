#include <stddef.h>
int main() {
    size_t num, size;
    void *ptr;

    void *c = calloc(num, size);  // UndCC_Valid
    void *m = malloc(size);       // UndCC_Valid
    void *r = realloc(ptr, size); // UndCC_Valid
    free(ptr);                    // UndCC_Valid

    return 0;
}
