// Effective C++

class Transaction {
public:
  Transaction()
  { init(); }                              // call to non-virtual...
  virtual void logTransaction() const = 0;
  // ...
private:
  void init()
  {
    // ...
    logTransaction(); // UndCC_Violation   // ...that calls a virtual!
  }
};
