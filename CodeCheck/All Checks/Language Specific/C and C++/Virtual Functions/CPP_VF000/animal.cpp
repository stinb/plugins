#include <iostream>
using namespace std;

/* Example of invalid use, Animal class contains a virtual method but no virtual destructor */
class Animal { // UndCC_Violation
  
  public: 
    Animal() {
      cout << "Constructing animal" << endl;
    }
    
    virtual void hunt() {
      cout << "Hunting for food!" << endl;
    }
    
    ~Animal() {
      cout << "Desctructing animal" << endl;
    }
  
};


/* Example of valid use, Furniture class contains both a virtual method and a virtual destructor */
class Furniture {
  
public:  
  Furniture() {
    cout << "Constructing furniture" << endl;
  }
  
  virtual void collapse() {
    cout << "Collapsing!" << endl;
  }
  
  virtual ~Furniture() {
    cout << "Desctructing furniture" << endl;
  }
  
};

/* Example of valid class, no virtual functions and no virtual destructor */
class Car {
  
  public: 
    Car() {
      cout << "Constructing car" << endl;
    }
    
    void engine_light() {
      cout << "Engine light turning on!" << endl;
    }
    
    ~Car() {
      cout << "Desctructing car" << endl;
    }
  
};
