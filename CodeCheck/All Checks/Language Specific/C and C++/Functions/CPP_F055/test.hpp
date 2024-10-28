class Base
{
public:
  virtual void goodExample( int param = 0 );
  virtual void badExample( int param = 0 );
};
class Derived : public Base
{
public:
  virtual void goodExample( int param = 0 );
  virtual void badExample( int param = 10 ); // UndCC_Violation
};
void foo( Derived& derivedObj )
{
  Base& baseObj = derivedObj;
// Acceptable, both statements call Derived::goodExample with a = 0.
  baseObj.goodExample();
  derivedObj.goodExample();
// Unacceptable, the first statement calls Derived::badExample with a = 0,
// the second statement calls Derived::badExample with a = 10, and may produce
// unexpected results.
  baseObj.badExample();
  derivedObj.badExample();
}
