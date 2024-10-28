class State {
public:
  
  State(int n) : size(n) { data = new double[size]; } // UndCC_Violation
  State operator+ (const State &rhs);
private:
  int size;
  double *data;
};

class State1 {
public:
  // UndCC_Valid
  explicit State1(int n) : size(n) { data = new double[size]; }
  State1 operator+ (const State1 &rhs);
private:
  int size;
  double *data;
};
