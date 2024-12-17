// Custom

// OKAY: no calls
class Okay1 {
  Okay1() {}
  ~Okay1() {}
};

// OKAY: not a call to a virtual function
class Okay2 {
  Okay2()
  {
    mem1();
  }
  ~Okay2()
  {
    mem1();
  }
  void mem1() {}
};

// OKAY: calls virtual function of another object
class Okay3 {
  Okay3()
  {
    Okay3 obj;
    obj.mem1();
  }
  ~Okay3()
  {
    Okay3 obj;
    obj.mem1();
  }
  virtual void mem1() {}
};

// OKAY: calls virtual function of another object
class Help_Okay4 {
public:
  virtual void virt1() {}
};
class Okay4 {
  Help_Okay4 *mem1;
  Okay4(Help_Okay4 *p1): mem1(p1)
  {
    mem1->virt1();
  }
  ~Okay4()
  {
    mem1->virt1();
  }
};

// OKAY: calls virtual function of another object
class Okay5 {
  Okay5 *ptr;
  Okay5(Okay5 *p1): ptr(p1)
  {
    ptr->virt1();
  }
  ~Okay5()
  {
    ptr->virt1();
  }
  virtual void virt1() {}
};

class Violation1 {
  Violation1()
  {
    virt1(); // UndCC_Violation
  }
  ~Violation1()
  {
    virt1(); // UndCC_Violation
  }
  virtual void virt1() {}
};

class Help_Violation2 {
public:
  virtual void virt1() {}
};
class Violation2: public Help_Violation2 {
  Violation2()
  {
    virt1(); // UndCC_Violation
  }
  ~Violation2()
  {
    virt1(); // UndCC_Violation
  }
  void virt1() {}
};

class Help_Violation4 {
};
class Violation4: public Help_Violation4 {
  Violation4()
  {
    virt1(); // UndCC_Violation
  }
  ~Violation4()
  {
    virt1(); // UndCC_Violation
  }
  virtual void virt1() {}
};

class Violation5;
static bool initPtr5(Violation5 *self);
static bool initRef5(Violation5 &self);
class Help_Violation5 {
public:
  virtual bool init()
  {
    return true;
  }
};
class Violation5: public Help_Violation5 {
public:
  const bool mInitialized;

  Violation5()
    : mInitialized(initPtr5(this)) // Violation is below in initPtr5
  {}

  explicit Violation5(int x)
    : mInitialized(initRef5(*this)) // Violation is below in initRef5
  {}

  virtual bool init() override
  {
    return false;
  }
};
static bool initPtr5(Violation5 *self)
{
  return self->init(); // UndCC_Violation
}
static bool initRef5(Violation5 &self)
{
  return self.init(); // UndCC_Violation
}

class Help_Violation6 {
public:
  virtual void virt1() {}
};
class Violation6: public Help_Violation6 {
  Violation6()
  {
    virt1(); // UndCC_FalseNeg - violation even though virtual dispatch isn't bypassed
  }
  ~Violation6()
  {
    virt1(); // UndCC_FalseNeg - violation even though virtual dispatch isn't bypassed
  }
};
