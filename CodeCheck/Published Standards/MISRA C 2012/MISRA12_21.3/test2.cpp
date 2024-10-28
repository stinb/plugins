#include <cstdlib>

int main() {
    size_t num, size;
    void *ptr;

    void *c = calloc(num, size);  // UndCC_Violation
    void *m = malloc(size);       // UndCC_Violation(0)
    void *r = realloc(ptr, size); // UndCC_Violation(0)
    free(ptr);                    // UndCC_Violation(0)

    return 0;
}
