struct stag_57
{
    unsigned short a_57;
    unsigned short b_57;
};

struct stag_57 a1_57 = { 0, 0 };    /* UndCC_Valid - compatible with above */
/* union stag_57 a2_57 = { 0, 0 };   * Analyis error - declares different type
                                     * from struct stag.
                                     * Constraint violation in C99 */

struct deer_57
{
    unsigned short a_57;
    unsigned short b_57;
};

void foo_57 ( void )
{
    struct deer_57                  /* UndCC_Violation */
    {
        unsigned short a_57;
    };
}

typedef struct coord_57
{
    unsigned short x_57;
    unsigned short y_57;
} coord_57;                         /* UndCC_Valid by Exception */

struct elk_57
{
    unsigned short x_57;
};

void bar_57 ( void )
{
    struct elk_57                   /* UndCC_Violation - declaration of different type
                                     * Constraint violation in C99 */
    {
        unsigned int x_57;
    };
}

enum T_57 {
    Up_57,
    Down_57
};
