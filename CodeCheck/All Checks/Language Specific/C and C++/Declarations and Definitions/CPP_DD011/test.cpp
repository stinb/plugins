#include <vector>

class Array {
public:
  Array(int n) : data(std::vector<int>(n)) {}
  Array(const Array &rhs) : data(rhs.data) { }

  // UndCC_Valid
  int &operator[] (int i) { return data[i]; }
  const int &operator[] (int i) const { return data[i]; }
private:
  std::vector<int> data;
};


class Array1 {
public:
  Array1(int n) : data(std::vector<int>(n)) {} // UndCC_Violation
  Array1(const Array1 &rhs) : data(rhs.data) { }


  int &operator[] (int i) { return data[i]; }
private:
  std::vector<int> data;
};
int main() {
  Array a(3); // Declare Array of size 3
  a[0] = 0; a[1] = 1; a[2] = 2; // Set a=[1,2,3] using non-const subscript operator
  int i = a[0]; // Uses the non-const subscript operator to set I
  const Array b(a); // b = a (or b = [1,2,3])
  int j = b[0];
  // b[0] = 4; // Good compile error! This should never work on a const object
}
