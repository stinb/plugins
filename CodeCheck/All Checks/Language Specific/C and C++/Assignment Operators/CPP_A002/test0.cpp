// Effective C++ (3rd Edition) Scott Meyers - Item 10

class Widget {
public:
  Widget& operator=(const Widget& rhs)
  {
    return *this;
  }

  Widget& operator+=(const Widget& rhs)
  {
    return *this;
  }

  Widget& operator=(int rhs)
  {
    return *this;
  }
};
