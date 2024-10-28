// Test cases for format #0:
//  xxx operator=(xxx C &p) { [return;] }

// Okay
class Okay0a {
public:
  int a;
  void operator=(Okay0a &rhs) {}
};

// Okay
class Okay0b {
public:
  int a;
  void operator=(const Okay0b &rhs)
  {
    return;
  }
};

