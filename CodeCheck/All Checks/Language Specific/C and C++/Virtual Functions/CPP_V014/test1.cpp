// Custom

class D
{
public:
   virtual void foo ( )
   {
   }
};

class E : public D
{
public:
   virtual void foo ( ) = 0;   // UndCC_Violation
};
