#include <string.h>
#include <stddef.h>

typedef short int16_t;
typedef int int32_t;

/* MISRA C 2025 worked example — union member assignment. */
void fn(void)
{
    union
    {
        int16_t i;
        int32_t j;
    } a = {0};

    a.i = a.i; /* UndCC_Valid - same member, exception 1 */
    a.j = a.i; /* UndCC_Violation */
}

/* MISRA C 2025 worked example — memcpy on overlapping slices of a plain C array. */
int16_t arr[20];

void f(void)
{
    memcpy(&arr[5], &arr[4], 2u * sizeof(arr[0]));  /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memmove(&arr[5], &arr[4], 2u * sizeof(arr[0])); /* UndCC_Valid - memmove permitted by exception 2 */
}

/* MISRA C 2025 worked example — exception 1: assignment between two objects
 * that overlap exactly and have compatible types is permitted. */
void g(void)
{
    int16_t *p = &arr[0];
    int16_t *q = &arr[0];

    *p = *q; /* UndCC_Valid - exception 1 */
}

/* Same array, different offsets — flagged for the conservative subset. */
void memcpy_same_array(void)
{
    int16_t buf[20];

    memcpy(buf, buf, 10u * sizeof(buf[0]));          /* UndCC_Violation(win(memcpy),lin(memcpy)) - direct same-pointer copy */
    memcpy(&buf[0], &buf[1], 10u * sizeof(buf[0]));  /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memcpy(&buf[1], &buf[0], 10u * sizeof(buf[0]));  /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memcpy(&buf[0], &buf[10], 5u * sizeof(buf[0]));  /* UndCC_Valid - disjoint slices, no overlap */
    memmove(&buf[0], &buf[1], 10u * sizeof(buf[0])); /* UndCC_Valid - memmove permitted */
}

/* Constant slices of one array, overlapping only when they are closer than the count. */
#define DELAY   (4)
#define OFFSET  (20)

void memcpy_constant_slices(int16_t *p, size_t n)
{
    int32_t a[32] = {0};
    int32_t m[4][8];

    memcpy(a, &a[OFFSET], DELAY * sizeof(int32_t));     /* UndCC_Valid - a[0..3] and a[20..23] */
    memcpy(a + 4, a, 4u * sizeof(a[0]));                /* UndCC_Valid */
    memcpy(&a[8], &a[4], sizeof(*a) * 4u);              /* UndCC_Valid */
    memcpy(&a[8], a, 4u * 2u * sizeof(a[0]));           /* UndCC_Valid */
    memcpy(&a[8], a, sizeof(a[0]) * 4u * 2u);           /* UndCC_Valid */
    memcpy(&a[7], a, sizeof(a[0]) * 4u * 2u);           /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memcpy(a, &a[2], sizeof(a[0]) * 4u - 3u);           /* UndCC_Violation(win(memcpy),lin(memcpy)) - 13 bytes, not 1 element */
    memcpy(&a[4], &a[1], 4u * sizeof(a[0]));            /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memcpy(a, &a[OFFSET - 18], DELAY * sizeof(int32_t)); /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memcpy(a, &a[20], 4u);                              /* UndCC_Violation(win(memcpy),lin(memcpy)) - a byte count, element size unknown */
    memcpy(a, &a[20], n * sizeof(a[0]));                /* UndCC_Violation(win(memcpy),lin(memcpy)) - count unknown */
    memcpy(a, &a[20], 4u * sizeof(int16_t));            /* UndCC_Violation(win(memcpy),lin(memcpy)) - not the element size */
    memcpy(m[0], m[1], 8u * sizeof(int32_t));           /* UndCC_FalsePos(win(memcpy),lin(memcpy)) - disjoint rows */
    memcpy(&m[0], &m[2], 2u * sizeof(m[0]));            /* UndCC_Valid */
    memcpy(&m[0], &m[1], 2u * sizeof(m[0]));            /* UndCC_Violation(win(memcpy),lin(memcpy)) */
    memcpy(p, &p[10], 10u * sizeof(int16_t));           /* UndCC_Valid */
    memcpy(p, &p[9], 10u * sizeof(int16_t));            /* UndCC_Violation(win(memcpy),lin(memcpy)) */
}

/* Different arrays — no overlap. */
void memcpy_distinct_arrays(void)
{
    int16_t src[10];
    int16_t dst[10];

    memcpy(dst, src, 10u * sizeof(src[0])); /* UndCC_Valid - distinct objects */
}

/* Real-world false-positive guard: destination object appears in the size
 * argument too, but the source is a distinct pointer. Must NOT flag —
 * the size arg is not a source-like operand. */
struct session
{
    char *buf;
    unsigned long len;
};

void memcpy_dest_in_size_arg(struct session *s, const char *src)
{
    memcpy(s->buf, src, s->len); /* UndCC_Valid - s appears in dest and size, but src is distinct */
}

void strncpy_dest_in_size_arg(char *dest, const char *src, unsigned long *n)
{
    strncpy(dest, src, *n); /* UndCC_Valid - n is the count, not a source */
}

/* Real-world false-positive guard: both args dereference different sub-objects
 * or pointer fields of the same parent struct. The root entity matches but
 * the actual memory regions are distinct. Must NOT flag. */
struct ctx
{
    char *path_lock;
    char *path_original;
    unsigned long len;
};

void memcpy_distinct_pointer_fields(struct ctx *file, unsigned long path_len)
{
    memcpy(file->path_lock, file->path_original, path_len); /* UndCC_Valid - distinct pointer fields */
}

struct caches
{
    int info;
    char *buffer;
};

void memcpy_addrof_member_vs_field(struct caches *sobj)
{
    memcpy(&sobj->info, sobj->buffer, sizeof(int)); /* UndCC_Valid - &member vs pointer field */
}

/* Nested union — assignment between a struct member of one union arm and the
 * other union arm (mirrors the M0-2-1 example translated to C). */
struct s1
{
    int16_t m1[32];
};

struct t1
{
    int32_t m2;
    struct s1 m3;
};

void nested_union(void)
{
    union
    {
        struct s1 u1;
        struct t1 u2;
    } a;

    a.u2.m3 = a.u1; /* UndCC_Violation */
}

/* Standard library string functions — overlap forbidden by the rule.
 * Gated by the str_funcs option so MISRA C++ 8.18.1 (which only names memcpy)
 * doesn't flag them by default. */
void str_funcs_same_ptr(char *p)
{
    strcpy(p, p);      /* UndCC_Violation(win(str_funcs),lin(str_funcs)) */
    strncpy(p, p, 10); /* UndCC_Violation(win(str_funcs),lin(str_funcs)) */
    strcat(p, p);      /* UndCC_Violation(win(str_funcs),lin(str_funcs)) */
    strncat(p, p, 5);  /* UndCC_Violation(win(str_funcs),lin(str_funcs)) */
}

void str_funcs_distinct(char *dest, const char *src)
{
    strcpy(dest, src);      /* UndCC_Valid - distinct pointers */
    strncpy(dest, src, 10); /* UndCC_Valid */
    strcat(dest, src);      /* UndCC_Valid */
    strncat(dest, src, 5);  /* UndCC_Valid */
}

/* sprintf/snprintf — destination passed back as a variadic argument. */
void sprintf_overlap(char *buf)
{
    sprintf(buf, "%s", buf);       /* UndCC_Violation(str_funcs) */
    snprintf(buf, 100, "%s", buf); /* UndCC_Violation(str_funcs) */
}

void sprintf_ok(char *buf, const char *other)
{
    sprintf(buf, "%s", other);       /* UndCC_Valid */
    snprintf(buf, 100, "%s", other); /* UndCC_Valid */
}
