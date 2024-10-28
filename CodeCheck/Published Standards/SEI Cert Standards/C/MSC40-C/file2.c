/* file2.c */

/* Static inline definition of get_random function */
static inline unsigned int get_random(void)
{
    /*
     * Initialize the seeds.
     * No more constraint violation; the inline function is now
     * internally linked.
     */
    static unsigned int m_z = 0xdeadbeef;
    static unsigned int m_w = 0xbaddecaf;

    /* Compute the next pseudorandom value and update the seeds  */
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

int main(void)
{
    /* Generate pseudorandom numbers using get_random()... */
    return 0;
}