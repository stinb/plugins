// MISRA C++ 2008

template < typename T > // UndCC_Violation
class Sample
{
public:
  void inst_mem ( )
  {
    // ...
  }
  void uninst_mem ( )  // UndCC_Violation
  {
    // ...
  }
};
int main () {
  Sample<long int> s;
  s.inst_mem();      // Call to s.inst_mem instantiates the member.
                             // s.uninst_mem is not called within the program
                             // and is not instantiated.
  return 0;
}
