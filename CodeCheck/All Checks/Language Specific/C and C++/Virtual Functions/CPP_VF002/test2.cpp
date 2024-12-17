// Effective C++

#include <string>

class Transaction {
public:
  explicit Transaction(const std::string& logInfo);
  void logTransaction(const std::string& logInfo) const; // now a non-
                                                         // virtual func
  // ...
};

Transaction::Transaction(const std::string& logInfo)
{
  // ...
  logTransaction(logInfo);                               // now a non-
}                                                        // virtual call

class BuyTransaction: public Transaction {
public:
  BuyTransaction( /* parameters */ )
  : Transaction(createLogString( /* parameters */ ))     // pass log info
  { /* ... */ }                                          // to base class
                                                         // ... // constructor
private:
  static std::string createLogString( /* parameters */ );
};
