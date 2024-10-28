  class A 
  { 
  public: 
     virtual void g(); 
     virtual void b(); 
  };
  class B1 : public A 
  { 
  public: 
     virtual void g();  // UndCC_Valid     - explicitly declared "virtual" 
     void b();           // UndCC_Violation - implicitly virtual 
  };
