// Test: no virtual functions can be called from constructors or destructors.

// OKAY: no calls
class Okay1 {
  Okay1() {};
  ~Okay1() {};
};

// OKAY: no calls to virtual functions
class Okay2 {
  Okay2() { mem1(); }
  ~Okay2() { mem1(); }
  void mem1() {};
};

// OKAY: calls virtual functions, but not via pointer
class Okay3 {
  Okay3() { Okay3 obj; obj.mem1(); }
  ~Okay3() { Okay3 obj; obj.mem1(); }
  virtual void mem1() {};
};

// OKAY: calls virtual functions, not in the current class
class Help_Okay4 { public: virtual void virt1() {}; };
class Okay4 {
  Help_Okay4 *mem1;
  Okay4(Help_Okay4 *p1) { mem1=p1; mem1->virt1(); }
  ~Okay4() { mem1->virt1(); }
};

// OKAY: calls virtual function of base class with no local implementation
class Help_Okay5 { public: virtual void virt1() {}; };
class Okay5: public Help_Okay5 {
  Okay5() { virt1(); }
  ~Okay5() { virt1(); }
};



class Violation1 {
  ~Violation1() { virt1(); } // UndCC_Violation
  virtual void virt1() {};
};


class Help_Violation2 { public: virtual void virt1() {}; };
class Violation2: public Help_Violation2 {
  ~Violation2() { virt1(); } // UndCC_Violation
  void virt1() {};
};


class Violation3 {
  Violation3 *ptr;
  ~Violation3() { ptr->virt1(); } // UndCC_Violation
  virtual void virt1() {}
};
