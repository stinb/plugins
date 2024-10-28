// Effective C++


#include <vector>


class Widget {
public:
  // ...
  ~Widget() { /* ... */ } // assume this might emit an exception
};
void doSomething()
{
  std::vector<Widget> v;
  // ...
}                         // v is automatically destroyed here


class DBConnection {
public:
  // ...
  static DBConnection create(); // function to return
                                // DBConnection objects; params
                                // omitted for simplicity

  void close() noexcept(false); // close connection; throw an
};                              // exception if closing fails


class DBConn {  // class to manage DBConnection
public:         // objects
  // ...
  ~DBConn()     // make sure database connections
  {             // are always closed
    db.close(); // UndCC_Violation
  }
private:
  DBConnection db;
};
