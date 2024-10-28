class Base {
private:
int x;
public:
virtual void mf1() = 0;
virtual void mf2();
void mf3();

};
class Derived: public Base {
public:
  virtual void mf1(); /* UNDCC_Violation */
  void mf4(); 
  
};

class Base1 {
private:
  int x;
public:
  virtual void mf1() = 0;
  virtual void mf1(int);
  virtual void mf2();
  void mf3();
  void mf3(double);

};
class Derived1: public Base1 {
public:
  virtual void mf1(); /* UNDCC_Violation */
  void mf3(); /* UNDCC_Violation */
  void mf4();
  
};

class Base2 {
public:
  virtual void mf1() = 0;
  virtual void mf1(int);

};
class Derived2: private Base2 {
public:
  virtual void mf1()
  { Base2::mf1(); } /* UNDCC_Valid */
  
};
