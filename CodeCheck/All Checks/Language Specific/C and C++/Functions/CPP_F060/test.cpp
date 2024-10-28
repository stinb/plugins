#include <iostream>
#include <string>

using namespace std;

// Customer Class
class Customer {

public:

  // class Variables
  string name;
  mutable string placedorder;
  int tableno;
  int bill;

  // constructor
  Customer(string s, string m, int a, int p)
  {
    name = s;
    placedorder = m;
    tableno = a;
    bill = p;
  }

  // to change the place holder
  void changePlacedOrder(string p) const
  {
    placedorder=p;
  }

  // change the bill (violation: this should be declared 'const', as it changes the value of non-mutable public member 'bill')
  void changeBill(int s) { bill = s; } // UndCC_Violation

  // to display
  void display() const
  {
    cout << "Customer name is: " << name << endl;
    cout << "Food ordered by customer is: "
         << placedorder << endl;
    cout << "table no is: " << tableno << endl;
    cout << "Total payable amount: " << bill << endl;
  }
};



int main()
{
  const Customer c1("Joe Smith", "Hamburger", 3, 100);
  c1.display();
  c1.changePlacedOrder("Cheeseburger");
  // c1.changeBill(150);
  c1.display();
  return 0;
}
