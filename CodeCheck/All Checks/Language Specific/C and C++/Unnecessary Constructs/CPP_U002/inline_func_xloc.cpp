// Test: check if inline function length exceeds 5 lines.

void func();

inline void Okay1() {}
inline void Okay2() {
  func(); // 1
  func(); // 2
  func(); // 3
  func(); // 4
  func(); // 5
}
class Okay {
  void Okay3() {
    func(); // 1
    func(); // 2
    func(); // 3
    func(); // 4
    func(); // 5
  };
  void Okay4();
};
Okay::Okay4() {
  func(); // 1
  func(); // 2
  func(); // 3
  func(); // 4
  func(); // 5
  func(); // 6
};

inline void Violation1() { // UndCC_Violation
  func(); // 1
  func(); // 2
  func(); // 3
  func(); // 4
  func(); // 5
  func(); // 6
}
class Violation {
  void Violation2() { // UndCC_Violation
    func(); // 1
    func(); // 2
    func(); // 3
    func(); // 4
    func(); // 5
    func(); // 6
  }
};
