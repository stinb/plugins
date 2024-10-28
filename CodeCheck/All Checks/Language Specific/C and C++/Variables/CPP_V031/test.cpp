class Test1
{
public:
  Test1(){}
  virtual ~Test1() {};
};

class Test11 {};

class Test2
{
public:
  Test2(){}
  virtual ~Test2() {};
};

class Test22 : public Test2
{
};



int main()
{  
  Test2 *t2;
  Test22 *t22 = static_cast<Test22*>(t2); // UndCC_Violation
  
  int a = 0;
  const int b = static_cast<int>(a); // UndCC_Violation
  
  float f = 4.5;
  int c = static_cast<int>(f); // UndCC_Valid

  return 0;
}
