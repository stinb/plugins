/* 5.8externalIdentifiersUniqueB.c */

static void foo_58 ( void ) /* UndCC_Violation - "foo" is not unique */
{ 							/* (it is already defined with external
							 * linkage in file1.c) */
	short count_58; 		/* UndCC_Violation - "count" has no linkage
							 * but clashes with an identifier with
							 * external linkage */
	int index_58;			/* UndCC_Valid - "index" has no linkage */
}
