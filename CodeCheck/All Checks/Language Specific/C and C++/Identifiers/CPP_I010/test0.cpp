// MISRA C++ 2008

namespace NS1
{
   static int global = 0;
}

namespace NS2
{
   void fn ( )
   {
      int global; // UndCC_Violation
   }
}
