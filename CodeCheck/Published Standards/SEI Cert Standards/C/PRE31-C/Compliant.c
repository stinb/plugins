#define ABS(x) (((x) < 0) ? -(x) : (x)) /* UNSAFE */

void func(int n)
{
    /* Validate that n is within the desired range */
    ++n;
    int m = ABS(n);

    /* ... */
}