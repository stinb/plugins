// MISRA C++ 2008

class A
{
public:
   virtual void f1 ( ) = 0;   // f1 is pure
   virtual void f2 ( ) = 0;   // f2 is pure
   virtual void f3 ( ) { }    // f3 is not pure
   virtual void f4 ( ) = 0;   // f4 is pure

   virtual ~A();              // destructor
};

// A::f1 is both pure and has a definition
void A::f1 ( )
{
}

// A::f4 is both pure and has a definition
void A::f4 ( )
{
}

class B : public A
{
public:
   virtual void f2 ( ) { }    // UndCC_Valid: f2 pure in A and
                              //            defined in B
   virtual void f3 ( ) { }    // UndCC_Violation: f3 defined in A and B
   virtual void f4 ( ) = 0;   // UndCC_Valid: f4 is pure in A and in B

   virtual ~B();              // UndCC_Valid: destructor
};

// UndCC_Valid by Exception - f4 defined in A but also declared pure in A
void B::f4 ( )
{
}

class C : public B
{
public:
   virtual void f1 ( ) { } // UndCC_Valid: f1 defined in A and C
                           //            but was pure in A
   virtual void f2 ( ) { } // UndCC_Violation f2: defined in B and C
                           // and not declared pure in B
   virtual void f4 ( ) { } // UndCC_Valid by Exception: f4 defined in A
                           // and B but also declared pure in A and B
};

class D : public C
{
public:
   virtual void f1 ( ) { } // UndCC_Violation f1: defined in C and D
                           // as well as A
   virtual ~D();           // UndCC_Valid: destructor
};

// Call by dominance example
class V
{
public:
   virtual void foo ( )
   {
   }
};

class B1 : public virtual V
{
public:
   virtual void foo ( )  // UndCC_Violation
   {
   }
};

class B2 : public virtual V
{
public:
   void f1 ( )
   {
      foo();  // V::foo would appear to be the only
              // canadidate to be called here
   }
};

class D1 : public B1, public B2
{
public:
   void f2 ( )
   {
      f1();
   }
};

void test ( )
{
   B2 b2;

   b2.f1();  // calls V::foo by normal inheritance rules

   D d;

   d.f2();   // calls B2::f1 which now calls B1::foo
             // "by dominance"
   d.f1();   // also calls B1::foo "by dominance"
}
