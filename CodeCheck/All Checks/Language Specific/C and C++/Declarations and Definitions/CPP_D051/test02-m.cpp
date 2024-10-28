template <typename T> T myMax(T x, T y) // UNDCC_Valid
{
  int b;                       // UndCC_Valid
  static void* a;              // Changed from void static* to static void*
  return (x > y)? x: y;
}

class Z
{
  typedef int hello();          // UNDCC_Valid
};

class Y : public Z
{
public:
  Y();                       // UNDCC_Valid
  virtual ~Y() = 0;             // Removed {} to make it a pure virtual function
  virtual void CMD() {};        // UNDCC_Valid
  int EX(int g, int h)          // Removed register keyword, it's deprecated in C++11
  {return 0;};
private:
  int* const a;                 // Changed from int mutable* to int* const
};

int main()
{
  static int e = 5;             // UNDCC_Valid
  int* x = &e;                  // Removed thread_local keyword, it's not allowed with pointers
  static char i, j, z;          // Added static keyword to make it compile
}
