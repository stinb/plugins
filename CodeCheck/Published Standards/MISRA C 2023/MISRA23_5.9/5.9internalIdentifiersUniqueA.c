/* 5.9internalIdentifiersUniqueA.c */

static int count_59; // UndCC_Violation - "count" has no linkage but clashes with an identifier with internal linkage
static void foo_59 ( void ) // UndCC_Violation - "foo" has internal linkage but clashes with a function of the same name
{
	short count_59; // UndCC_Violation - "count" has internal linkage but clashes with other identifiers of the same name
	short index_59; 		/* "index" has no linkage */
}

void bar1_59 ( void )
{
	static short count_59; 	/* UndCC_Violation - "count" has no linkage
							 * but clashes with an identifier with
							 * internal linkage */
	short index_59; 		/* UndCC_Valid - "index" is not unique but
							 * has no linkage */
	foo_59 ( );
}

/* 5.9internalIdentifiersUniqueA.c */
