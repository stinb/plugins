class A {};

class Okay : public A
{
public:
  Okay (int a, int b, int c) : one (a), two (b), three (c)  {}    

  int one;
  int two;
  int three;
};

class Bad : public Okay
{
  Bad (int d) : four (d), Okay(1,2,3) {} // UndCC_Violation
  int four;
};
