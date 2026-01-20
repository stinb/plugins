struct Point { // UndCC_Valid - implicit functions
  int x;
  int y;
};

void f()
{
  Point a;
}

void f(int = 0); // UndCC_Valid - unnamed default parameter
