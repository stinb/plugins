// Minimal stand-in for <typeinfo> (header not available in the analysis
// environment); the typeid operator only needs std::type_info to exist.
namespace std { class type_info {}; }

class B1_1211
{
public:
  B1_1211()
  {
    typeid(B1_1211); // UndCC_Valid, B1 not polymorphic
  }
};
class B2_1211
{
public:
  virtual ~B2_1211();
  virtual void foo_1211();
  B2_1211()
  {
    typeid(B2_1211);               // UndCC_Violation
    B2_1211::foo_1211();           // UndCC_Valid � not a virtual call
    foo_1211();                    // UndCC_Violation
    dynamic_cast<B2_1211 *>(this); // UndCC_Violation
  }
};

// MISRA23_15.1.1 Test
class B2
{
public:
  virtual ~B2();
  virtual void foo();
  virtual void goo() = 0;
  void bar()
  {
    foo();
    typeid(*this);
  }
  B2()
  {
    typeid(*this);            // UndCC_Violation
    typeid(B2);               // UndCC_Valid - current object type not used
    B2::foo();                // UndCC_Valid - not a virtual call
    foo();                    // UndCC_Violation
    goo();                    // UndCC_Violation - undefined behaviour
    dynamic_cast<B2 *>(this); // UndCC_Violation
    bar();                    // UndCC_Violation - indirect call to foo and
                              // use of typeid on current object
  }
};
class B4;
class B3
{
public:
  explicit B3(B4 *b);
  virtual ~B3();
  virtual void foo();
};
class B4 : public B3
{
public:
  B4() : B3(this) {}
};
B3::B3(B4 *b)
{
  foo();       // UndCC_Violation - calls B3::foo
  this->foo(); // UndCC_Violation - calls B3::foo
  b->foo();    // UndCC_Violation - undefined behaviour
}

// Issue #4878: Qt-style member-of-member virtual call should not be flagged.
// The receiver is an unrelated polymorphic object, not the object under
// construction, so the dynamic type of the constructed object is not used.
class QPushButton4878
{
public:
  virtual void setVisible(bool v);
};

class UiWrenchWidget4878
{
public:
  QPushButton4878 *resetButton;
};

class WrenchWidget4878
{
public:
  UiWrenchWidget4878 *ui;
  WrenchWidget4878()
  {
    ui->resetButton->setVisible(false); // UndCC_Valid - call is on an unrelated object
  }
};