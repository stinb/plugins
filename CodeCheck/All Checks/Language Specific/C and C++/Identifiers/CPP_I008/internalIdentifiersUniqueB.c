/* 5.9internalIdentifiersUniqueB.c */

const char constTest = 1;
static char count_59;                           /* UndCC_FalseNeg - "count" has internal
                                                                         * linkage but clashes with other
                                                                         * identifiers of the same name */
static void foo_59 ( void )             /* UndCC_FalseNeg - "foo" has internal
                                                                         * linkage but clashes with a function of
                                                                         * the same name */
{
        int index_59;                                   /* UndCC_Valid - both "index" and "nbytes" */
        short nbytes_59;                                /* are not unique but have no linkage */
        extern int text_59;
}

void bar2_59 ( void )
{
        static unsigned char nbytes_59; /* UndCC_Valid - "nbytes" is not unique but
                                                                         * has no linkage and the storage class is
                                                                         * irrelevant */
}

/* End of 5.9internalIdentifiersUniqueB.c */
