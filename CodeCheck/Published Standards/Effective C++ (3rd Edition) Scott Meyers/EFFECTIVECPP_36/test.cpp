class B {
public:
void mf(); //UNDCC_Violation mf is also defined in class D

};
class D: public B { 
public:
  void mf() {}
};

int main() {
  D x; // x is an object of type D
  B *pB = &x; // get pointer to x
  D *pD = &x; // get pointer to x
  
  pB->mf(); // calls B::mf
  pD->mf(); // calls D::mf
}
