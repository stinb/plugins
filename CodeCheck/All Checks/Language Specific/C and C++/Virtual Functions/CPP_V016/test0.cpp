// Effective C++

class Transaction {                         // base class for all
public:                                     // transactions
  Transaction();
  virtual void logTransaction() const = 0;  // make type-dependent
                                            // log entry
  // ...
};

Transaction::Transaction()                  // implementation of
{                                           // base class ctor
  // ...
  logTransaction(); // UndCC_Violation      // as final action, log this
}                                           // transaction

class BuyTransaction: public Transaction {  // derived class
  public:
  virtual void logTransaction() const;      // how to log trans-
                                            // actions of this type
  // ...
};

class SellTransaction: public Transaction { // derived class
public:
  virtual void logTransaction() const;      // how to log trans-
                                            // actions of this type
  // ...
};
