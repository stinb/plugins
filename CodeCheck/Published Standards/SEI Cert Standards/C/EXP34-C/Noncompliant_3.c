struct s1
{
    /* data */
    int x;
    double y;
};

struct s2
{
    struct s1 *s2_s1;
    int z;
};

int foo(struct s2 *samp)
{
    struct s2 *samp2 = samp;
    struct s1 *samp3 = samp2->s2_s1; // UndCC_Violation
    unsigned int mask = 0;

    if (!samp2)
        return 0;

    return 1;
}
