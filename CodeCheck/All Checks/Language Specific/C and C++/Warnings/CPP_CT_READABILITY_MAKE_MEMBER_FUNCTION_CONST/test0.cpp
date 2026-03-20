// Custom

class Health {
public:
  operator unsigned() const // UndCC_Valid
  {
    return mValue;
  }

  operator unsigned() // UndCC_Violation
  {
    return mValue;
  }

  void damage(unsigned value) // UndCC_Valid
  {
    if (value <= mValue)
      mValue -= value;
    else
      mValue = 0;
  }

private:
  unsigned mValue;
};
