// $Id: A18-1-4.cpp 313638 2018-03-26 15:34:51Z jan.babst $
#include <memory>
class A
{
};
void F1()
{
  // Create a dynamically allocated array of 10 objects of type A.
  auto up1 = std::make_unique<A[]>(10); // UndCC_Valid

  std::unique_ptr<A> up2{up1.release()}; // UndCC_Violation
}
void F2()
{
  auto up1 = std::make_unique<A[]>(10); // UndCC_Valid

  std::unique_ptr<A> up2;
  up2.reset(up1.release()); 
}
void F3()
{
  auto up = std::make_unique<A[]>(10); // UndCC_Valid

  std::shared_ptr<A> sp{up.release()}; // UndCC_Violation
}

void F4()
{
  auto up = std::make_unique<A[]>(10); // UndCC_Valid

  std::shared_ptr<A> sp;
  sp.reset(up.release()); 
}
void F5()
{
  auto up = std::make_unique<A[]>(10); // UndCC_Valid

  // sp will obtain its deleter from up, so the array will be correctly
  // deallocated. However, this is no longer allowed in C++17.
    
  //sp.reset(new A{}); // leads to undefined behavior
}
void F6()
{
  auto up = std::make_unique<A[]>(10); // UndCC_Valid

  // Well behaving, but error-prone
  std::shared_ptr<A> sp{up.release(), // UndCC_Violation
  std::default_delete<A[]>{}}; 
  sp.reset(new A{}); // leads to undefined behavior
}
