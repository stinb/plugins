// $Id: A10-3-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $

class A
{
  public:
    virtual ~A() {} // UndCC_Valid
    virtual void F() noexcept = 0; // UndCC_Valid
    virtual void G() noexcept final = 0; // UndCC_Violation - virtual final pure

    // function is redundant
    virtual void
    H() noexcept final // UndCC_Violation - function is virtual and final
    {
    }
    
    virtual void K() noexcept // UndCC_Valid
    {
    }
    
    virtual void J() noexcept {} // UndCC_Valid
    
    virtual void M() noexcept // UndCC_Valid
    {
    }
    
    virtual void Z() noexcept // UndCC_Valid
    {
    }
    
    virtual A& operator+=(A const& rhs) noexcept // UndCC_Valid
    {
      // ...
      return *this;
    }
};

class B : public A
{
  public:
    ~B() override {} // UndCC_Valid
    
    virtual void F() noexcept override // UndCC_Violation - function is specified
    // with virtual and override
    {
    }
    
    void K() noexcept override // UndCC_Violation
    final 
    {
    }
    
    virtual void M() noexcept // UndCC_Valid - violates A10-3-2
    {
    }
    
    void Z() noexcept override // UndCC_Valid
    {
    }
    
    void J() noexcept // UndCC_Violation - virtual function but not marked as
    // overrider
    {
    }
    
    A& operator+=(A const& rhs) noexcept override // UndCC_Valid - to override
                                                  // the operator correctly,
                                                  // its signature needs to be
                                                  // the same as in the base
                                                  // class
    {
      // ...
      return *this;
    }
};
