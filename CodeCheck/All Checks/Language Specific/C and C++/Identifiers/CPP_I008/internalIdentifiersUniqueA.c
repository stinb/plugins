/* 5.9internalIdentifiersUniqueA.c */

static int count_59;            /* "count" has internal linkage */ // UndCC_Violation
static void foo_59 ( void ) /* "foo" has internal linkage */ // UndCC_Violation
{
        short count_59;                 /* UndCC_Violation - "count" has no linkage
                                                         * but clashes with an identifier with
                                                         * internal linkage */
        short index_59;                 /* "index" has no linkage */
}

void bar1_59 ( void )
{
        static short count_59;  /* UndCC_Violation - "count" has no linkage
                                                         * but clashes with an identifier with
                                                         * internal linkage */
        short index_59;                 /* UndCC_Valid - "index" is not unique but
                                                         * has no linkage */
        foo_59 ( );
}

/* 5.9internalIdentifiersUniqueA.c */
