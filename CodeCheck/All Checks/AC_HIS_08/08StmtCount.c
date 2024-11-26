int tooManyStatements() { // UndCC_Violation
  int array[100], n, c, d, swap;

  printf("Enter number of elements\n");
  scanf("%d", &n);

  printf("Enter %d integers\n", n);

  for (c = 0; c < n; c++)
    scanf("%d", &array[c]);

  for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] > array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }

  printf("Sorted list in ascending order:\n");

  for ( c = 0 ; c < n ; c++ )
     printf("%d\n", array[c]);

   int first, second, *p, *q, sum;

   printf("Enter two integers to add\n");
   scanf("%d%d", &first, &second);

   p = &first;
   q = &second;

   sum = *p + *q;

   printf("Sum of entered numbers = %d\n",sum);

    int year;

  printf("Enter a year to check if it is a leap year\n");
  scanf("%d", &year);

  if ( year%400 == 0)
    printf("%d is a leap year.\n", year);
  else if ( year%100 == 0)
    printf("%d is not a leap year.\n", year);
  else if ( year%4 == 0 )
    printf("%d is a leap year.\n", year);
  else
    printf("%d is not a leap year.\n", year);


int c2, first2, last, middle, n2, search, array2[100];

printf("Enter number of elements\n");
scanf("%d",&n);

printf("Enter %d integers\n", n);

for (c = 0; c < n; c++)
	scanf("%d",&array[c]);

printf("Enter value to find\n");
scanf("%d", &search);

first = 0;
last = n - 1;
middle = (first+last)/2;

while (first <= last) {
	if (array[middle] < search)
		first = middle + 1;
	else if (array[middle] == search) {
		printf("%d found at location %d.\n", search, middle+1);
		break;
	}
	else
		last = middle - 1;

	middle = (first + last)/2;
}
if (first > last)
	printf("Not found! %d is not present in the list.\n", search);

return 0;

}
