// Custom

class Okay1 {
public:
  Okay1& operator+=(const Okay1& rhs)
  {
    return((*((this))));
  }
};

class Okay2 {
public:
  using Ref = Okay2&;
  Ref operator+=(const Okay2& rhs)
  {
    return *this;
  }
};

namespace Okay3 {
  class Class {
  public:
    Class& operator+=(const Class& rhs)
    {
      return *this;
    }
  };
}

class Okay4 {
public:
  bool operator==(const Okay4& rhs)
  {
    return this == &rhs;
  }
};

// Return type is const
class Violation1 {
public:
  using ConstRef = const Violation1&;
  ConstRef operator*=(const Violation1& rhs) // UndCC_Violation
  {
    return *this;
  }
};

// Return type is not a reference to the class type
class Violation2 {
public:
  void operator<<=(const Violation2& rhs) // UndCC_Violation
  {

  }
};

// Returns something besides `this`
class Violation3 {
public:
  Violation3& operator=(const Violation3& rhs)
  {
    static Violation3 other;
    return other; // UndCC_Violation
  }
};

// Returns something besides `this`
class Violation4 {
public:
  Violation4& operator=(Violation4& rhs)
  {
    return rhs; // UndCC_Violation
  }
};

// Returns nothing
class Violation5 {
public:
  Violation5& operator=(const Violation5& rhs) // UndCC_Violation
  {

  }
};
