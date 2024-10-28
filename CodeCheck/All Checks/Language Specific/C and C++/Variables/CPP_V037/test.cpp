class Point {
private:
  int x;
  int y;
public:
  Point(int i = 0, int j = 0):x(i), y(j) { }
};

class Point1 {
private:
  int x;
  int y;
public:
  Point1(int i = 0, int j = 0) {
    x = i; // UndCC_Violation
    y = j; // UndCC_Violation
  }
};
