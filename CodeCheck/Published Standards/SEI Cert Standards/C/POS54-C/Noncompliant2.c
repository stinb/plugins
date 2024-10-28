#include <stdio.h>
#include <stdlib.h>

struct point {
    int x;
    int y;
};

int not_used(struct point p, struct point **memptr, size_t alignment, size_t size)
{
    // Call the POSIX library function
    int error = posix_memalign(memptr, alignment, size);

    // // Check for errors
    // if (error != 0) {
    //     return 1;
    // }

    // Continue and don't use memptr

    return 0;
}

int used(struct point p, struct point **memptr, size_t alignment, size_t size)
{
    // Call the POSIX library function
    int error = posix_memalign(memptr, alignment, size);

    // // Check for errors
    // if (error != 0) {
    //     return 1;
    // }

    // Continue and use memptr
    (*memptr)->x = 3; // UndCC_Violation
    (*memptr)->y = 27;
    printf("%d, %d\n", p.x, p.y);
    printf("%d, %d\n", (*memptr)->x, (*memptr)->y);

    return 0;
}

int main()
{
    int error;

    struct point p = { .x = 2, .y = 8 };
    struct point *mem = &p;
    struct point **memptr = &mem;
    size_t alignment = _Alignof memptr;
    size_t size = sizeof memptr;

    error = not_used(p, memptr, alignment, size);
    if (error)
        return 1;

    error = used(p, memptr, alignment, size);
    if (error)
        return 2;

    return 0;
}
