// MISRA C++ 2023 Rule 6.4.3
// Names from a dependent base shall be prefixed with this->,
// qualified with a using declaration, or qualified with the base name.

// Global entities that shadow base members
typedef int Type;
typedef int value_type;

void g() {}

int value = 0;

// Base class template
template <typename T>
struct Base {
  typedef T Type;
  typedef T value_type;
  void g() {}
  int value;
};


// Non-compliant: unqualified use of names from dependent base
template <typename T>
struct NonCompliant : Base<T> {
  value_type separator;                     // UndCC_Violation - unqualified type from base

  void f() {
    Type t = 0;                             // UndCC_Violation - unqualified type from base
    g();                                    // UndCC_Violation - unqualified call to base member
    int x = value;                          // UndCC_Violation - unqualified variable from base
  }
};


// Compliant: using declarations
template <typename T>
struct CompliantUsing : Base<T> {
  using typename Base<T>::Type;
  using typename Base<T>::value_type;
  using Base<T>::g;
  using Base<T>::value;

  value_type separator;                     // UndCC_Valid

  void f() {
    Type t = 0;                             // UndCC_Valid
    g();                                    // UndCC_Valid
    int x = value;                          // UndCC_Valid
  }
};


// Compliant: this-> prefix
template <typename T>
struct CompliantThis : Base<T> {
  void f() {
    this->g();                              // UndCC_Valid
    int x = this->value;                    // UndCC_Valid
  }
};


// Compliant: qualified with base name
template <typename T>
struct CompliantQualified : Base<T> {
  typename Base<T>::value_type separator;   // UndCC_Valid

  void f() {
    typename Base<T>::Type t = 0;           // UndCC_Valid
    Base<T>::g();                           // UndCC_Valid
    int x = Base<T>::value;                 // UndCC_Valid
  }
};


// Compliant: global qualifier
template <typename T>
struct CompliantGlobal : Base<T> {
  ::value_type separator;                   // UndCC_Valid

  void f() {
    ::Type t = 0;                           // UndCC_Valid
    ::g();                                  // UndCC_Valid
    int x = ::value;                        // UndCC_Valid
  }
};


// No shadow: name does not exist in base
template <typename T>
struct NoShadow : Base<T> {
  void f() {
    int localVar = 42;                      // UndCC_Valid - not in base
  }
};


// Non-dependent base: rule does not apply
struct ConcreteBase {
  typedef int Type;
  void g() {}
  int value;
};

struct NonTemplate : ConcreteBase {
  void f() {
    Type t = 0;                             // UndCC_Valid - non-dependent base
    g();                                    // UndCC_Valid
    int x = value;                          // UndCC_Valid
  }
};


// Method return type and parameter type from dependent base
template <typename T>
struct ReturnParamTypes : Base<T> {
  value_type getVal();                      // UndCC_Violation - return type
  void setVal(value_type v);               // UndCC_Violation - parameter type
};

template <typename T>
value_type ReturnParamTypes<T>::getVal()    // UndCC_FalseNeg - out-of-class definition is outside specialization scope
{
  return 0;
}


// Derived class defines its own member — should not flag
template <typename T>
struct OwnMember : Base<T> {
  void g() {}                               // UndCC_Valid - own member, not global g()

  void f() {
    g();                                    // UndCC_Valid - calls own g(), not global
  }
};


// Multi-level inheritance: grandparent has the name
template <typename T>
struct Mid : Base<T> {};

template <typename T>
struct GrandChild : Mid<T> {
  void f() {
    g();                                    // UndCC_Violation - name from grandparent base
    int x = value;                          // UndCC_Violation - name from grandparent base
  }
};


// CRTP pattern
template <typename Derived>
struct CRTPBase {
  void impl() {}
};

template <typename T>
struct CRTPDerived : CRTPBase<CRTPDerived<T>> {
  void f() {
    // impl() is in the base, but there's no global impl(), so no shadow
    int localVar = 42;                      // UndCC_Valid - no shadow
  }
};


// Explicit instantiations to trigger analysis
template struct NonCompliant<int>;
template struct CompliantUsing<int>;
template struct CompliantThis<int>;
template struct CompliantQualified<int>;
template struct CompliantGlobal<int>;
template struct NoShadow<int>;
template struct ReturnParamTypes<int>;
template struct OwnMember<int>;
template struct GrandChild<int>;
template struct CRTPDerived<int>;
