/* file2.c */

/* Inline definition of get_random function */
inline unsigned int get_random(void) // UndCC_Violation, should be static
{
    /*
     * Initialize the seeds
     * Constraint violation: static duration storage referenced
     * in non-static inline definition
     */
    static unsigned int m_z = 0xdeadbeef;
    static unsigned int m_w = 0xbaddecaf;

    /* Compute the next pseudorandom value and update the seeds */
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

int main(void)
{
    unsigned int rand_no;
    for (int ii = 0; ii < 100; ii++)
    {
        /*
         * Get a pseudorandom number. Implementation defined whether the
         * inline definition in this file or the external definition
         * in file2.c is called.
         */
        rand_no = get_random();
        /* Use rand_no... */
    }

    /* ... */

    /*
     * Get another pseudorandom number. Behavior is
     * implementation defined.
     */
    rand_no = get_random();
    /* Use rand_no... */
    return 0;
}