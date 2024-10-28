// Custom

int max(int x, int y)
{
  return (x >= y) ? x : y;
}

int main()
{
  int x = 3;
  int y = 6;

  x = max(x, y);
  x = max(max(x, y), y);
  x = max(x, max(x, y));
  x = max(x, max(max(x, y), y));
  x = max(max(x, y) + max(x, y), y);
  x = max(max(x, y), max(x, y));         // UndCC_Violation
  x = max(max(x, y) + x, max(x, y) + y); // UndCC_Violation
  x = max(x, max(max(x, y), max(x, y))); // UndCC_Violation

  return 0;
}
