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
class QObject4878
{
public:
  virtual ~QObject4878() {}
};

class QWidget4878 : public QObject4878
{
public:
  virtual void setVisible(bool v);
};

class QAbstractButton4878 : public QWidget4878
{
};

class QPushButton4878 : public QAbstractButton4878
{
};

class UiWrenchWidget4878
{
public:
  QPushButton4878 *resetButton;
  QPushButton4878 *helpButton;
  QPushButton4878 *driveOneTickMarks;

  // Non-virtual helper that itself makes virtual calls on its own members.
  // This is what Qt's auto-generated Ui_*::setupUi() does. It must not cause
  // a violation when called as ui->setupUi(parent) from the constructor,
  // because the call is on `ui`, not on the constructed widget.
  void setupUi(QWidget4878 *parent)
  {
    resetButton->setVisible(true);
  }
};

class WrenchWidget4878 : public QWidget4878
{
public:
  UiWrenchWidget4878 *ui;
  WrenchWidget4878(QWidget4878 *const MyParent)
    : QWidget4878()
    , ui(new UiWrenchWidget4878)
  {
    ui->setupUi(this);                         // UndCC_Valid - non-virtual call on an unrelated object
    ui->resetButton->setVisible(false);        // UndCC_Valid - virtual call on an unrelated object
    ui->helpButton->setVisible(false);         // UndCC_Valid - same pattern
    ui->driveOneTickMarks->setVisible(false);  // UndCC_Valid - same pattern (issue #4878 follow-up)
  }
};

// Issue #4878 follow-up: implicit-this call to a helper member function
// whose internal virtual calls all target an unrelated object should not
// be flagged. Mirrors TableViewDialog::showLegend() from the reporter.
class QGroupBox4878 : public QWidget4878
{
};

class UiTableViewDialog4878
{
public:
  QGroupBox4878 *legendGroupBox;
};

class TableViewDialog4878 : public QWidget4878
{
public:
  UiTableViewDialog4878 *ui;

  void showLegend(const bool Show) const
  {
    ui->legendGroupBox->setVisible(Show); // virtual call on unrelated object
  }

  TableViewDialog4878()
    : QWidget4878()
    , ui(new UiTableViewDialog4878)
  {
    showLegend(true); // UndCC_Valid - helper's inner virtual is not on `this`
  }
};

// Counter-test: an implicit-this helper that does call a virtual on `this`
// must still be flagged so we don't regress the legitimate-violation case.
class HelperOnThis4878
{
public:
  virtual void doIt();

  void helper()
  {
    doIt(); // implicit this->doIt() - virtual call on `this`
  }

  HelperOnThis4878()
  {
    helper(); // UndCC_Violation - indirect virtual call on the constructed object
  }
};