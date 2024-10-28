void f_814 ( void ) 
{
	int p_814[3];
	int q_814[3];
	int n_814;
	/* memcpy has restrict-qualified parameters */
	memcpy ( p_814, q_814, n_814 );
}

void user_copy_814 ( void * restrict p_814, void * restrict q_814, int n_814 )  // UndCC_Violation
{
}

int i;
i = 265;
