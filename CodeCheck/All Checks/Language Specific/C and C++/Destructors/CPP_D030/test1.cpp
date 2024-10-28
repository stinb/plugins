// Effective C++


#include <cstdlib>


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
  ~DBConn();    // make sure database connections
                // are always closed
private:
  DBConnection db;
};


DBConn::~DBConn()
{
  try { db.close(); }
  catch (...) {
    // make log entry that the call to close failed;
    std::abort();
  }
}
