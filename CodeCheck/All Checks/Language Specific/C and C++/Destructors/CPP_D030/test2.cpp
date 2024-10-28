// Effective C++


class DBConnection {
public:
  // ...
  static DBConnection create(); // function to return
                                // DBConnection objects; params
                                // omitted for simplicity

  void close() noexcept(false); // close connection; throw an
};                              // exception if closing fails



class DBConn {
public:
  // ...
  void close()                                       // new function for
  {                                                  // client use
    db.close();
    closed = true;
  }
  ~DBConn()
  {
    if (!closed) {
      try {                                          // close the connection
        db.close();                                  // if the client didn’t
      }
      catch (...) {                                  // if closing fails,
        // make log entry that call to close failed; // note that and
        // ...                                       // terminate or swallow
      }
    }
  }
private:
  DBConnection db;
  bool closed;
};
