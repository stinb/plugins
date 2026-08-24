#include <cstdint>

class C1
{
public:
   ~C1()
   {
      try
      {
         throw(42); // UndCC_Valid - exception will not leave
                    // destructor
      }
      catch (int32_t i) // int handler
      {
         // Handle int exception throw by destructor
      }
   }
};
class C2
{
public:
   ~C2()
   {
      throw(42); // UndCC_Violation - destructor exits with an
                 // exception
   }
};
void foo()
{
   C2 c; // program terminates when c is destroyed
   throw(10);
}