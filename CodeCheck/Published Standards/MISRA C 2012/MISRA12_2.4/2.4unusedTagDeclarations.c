void unusedtag ( void )
{
	enum state { S_init, S_run, S_sleep }; /* UndCC_Violation */
}

typedef struct record_t /* UndCC_Violation */
{
	unsigned short key;
	unsigned short val;
} record1_t;

typedef struct /* UndCC_Valid */
{
	unsigned short key;
	unsigned short val;
} record2_t;
