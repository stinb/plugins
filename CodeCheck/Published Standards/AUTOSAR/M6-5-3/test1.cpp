// Custom

void fn () {
  int x;

  for ( x = 0; x < 10; )
  {
    x = x * 2; // UndCC_Violation
  }
    for ( x = 0; x < 10; )
    {
      x = x * 2; // UndCC_Violation
    }
      for ( x = 0; x < 10; )
      {
        x = x * 2; // UndCC_Violation
      }
        for ( x = 0; x < 10; )
        {
          x = x * 2; // UndCC_Violation
        }
	for ( x = 0; x < 10; )
	{
		x = x * 2; // UndCC_Violation
	}
		for ( x = 0; x < 10; )
		{
			x = x * 2; // UndCC_Violation
		}
			for ( x = 0; x < 10; )
			{
				x = x * 2; // UndCC_Violation
			}
				for ( x = 0; x < 10; )
				{
					x = x * 2; // UndCC_Violation
				}
}
