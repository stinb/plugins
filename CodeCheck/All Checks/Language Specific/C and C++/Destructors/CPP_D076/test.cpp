
class Base { // UndCC_Violation
public:
  Base() {}
  ~Base(){}; // Destructor is not virtual
   
};
  
class Derived: public Base {
  public:
    Derived() {}
    ~Derived() {}
};

// UndCC_Valid
class Base1 {
public:
  Base1() {}
  virtual ~Base1(){}
};

class Derived1: public Base1 {
public:
  Derived1() {}
  ~Derived1() {}
};
