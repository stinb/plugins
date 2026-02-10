// Custom

void f(); // UndCC_Valid - Missing parameters are not deprecated in C++

struct Point {
  int x;
  int y;

  Point(); // UndCC_Valid - Missing parameters are not deprecated in C++
  Point(int x, int y);
};
