template < typename T > // UndCC_Violation
class Sample1471
{
public:
  void inst_mem1471 ( )
  {
    // ...
  }
  void uninst_mem1471 ( )  // UndCC_Violation
  {
    // ...
  }
};
int main () {
  Sample1471<long int> s1471;
  s1471.inst_mem1471();      // Call to s.inst_mem instantiates the member.
                             // s.uninst_mem is not called within the program
                             // and is not instantiated.
  return 0;
}
