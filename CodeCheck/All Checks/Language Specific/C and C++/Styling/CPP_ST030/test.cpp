// UndCC_Valid
#ifndef Class_nameH
#define Class_nameH
class Class_name {
private:
// private data and functions here...
  void copy(const Class_name &);
  void free();
public:
// Constructors and destructor
  Class_name();
  Class_name(const Class_name & obj) {
    copy (obj);
  }
// other constructors here ...
  ~Class_name() { free(); }
// Modifiers
  const Class_name & operator=(const Class_name & obj) {
    if (this != &obj) {
      free();
      copy(obj);
    }
    return *this;
  }
// other member functions here ...
// Selectors
// const member functions here ...
};
#endif



class ClassName { // UndCC_Violation
public:
  int a;
private:
  int b;
};
