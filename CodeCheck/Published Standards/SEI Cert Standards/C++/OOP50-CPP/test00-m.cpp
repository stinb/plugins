struct B {
  B() { seize(); } // UndCC_Violation
  virtual ~B() { release(); } // UndCC_Violation
  
protected:
  virtual void seize();
  virtual void release();
};


class BSafe {
  void seize_mine();
  void release_mine();
  
public:
  BSafe() { seize_mine(); }
  virtual ~BSafe() { release_mine(); }
  
protected:
  virtual void seize() { seize_mine(); }
  virtual void release() { release_mine(); }
};

 
struct D : B {
  virtual ~D() = default;
  
protected:
  void seize() override {
    B::seize();
    // Get derived resources...
  }
  
  void release() override {
    // Release derived resources...
    B::release();
  }
};


struct A {
  A() {
    // f();   // WRONG!
    A::f();   // Okay - OOP50-CPP-EX1
  }
  virtual void f();
};

struct X : A {
  X() : A() {
    f();  // Okay - OOP50-CPP-EX2
  }
  void f() override final;
};

struct Z final : A {
  Z() : A() {
    f();  // Okay - OOP50-CPP-EX2
  }
  void f() override;
};

