void f1_153 ( int a_153 )
{
	if ( a_153 <= 0 )
	{
		goto L2_153; 				/* UndCC_Violation */
	}
	goto L1_153; 					/* UndCC_Valid */
	if ( a_153 == 0 )
	{
		goto L1_153; 				/* C ompliant */
	}
	goto L2_153; 					/* UndCC_Violation */
L1_153:
	if ( a_153 > 0 )
	{
L2_153:
		goto L1_153;				/* UndCC_Valid */
	}
	{
		{
			goto L2_153;			/* UndCC_Violation */
		}
	}
	
	int x_153 = 1;
	int y_153 = 1;
	goto L4_153;					/* UndCC_Valid */
L5_153:
	switch ( x_153 )
	{
		case 0:
			if ( x_153 == y_153 )
			{
				goto L3_153;		/* UndCC_Violation */
			}
			break;
		case 1:
			y_153 = x_153;
L3_153:
			++x_153;
			break;
		case 2:
			goto L4_153;			/* UndCC_Valid */
			goto L5_153;			/* UndCC_Valid */
		case 3:
			goto L3_153;			/* UndCC_Violation */
		default:
			break;
	}
	
L4_153:


	return;
}
