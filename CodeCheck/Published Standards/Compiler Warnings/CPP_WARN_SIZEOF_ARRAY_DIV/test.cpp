int fn ()
{
  int arr[10];
  return sizeof (arr) / sizeof (short); // UNDCC_Violation
}
