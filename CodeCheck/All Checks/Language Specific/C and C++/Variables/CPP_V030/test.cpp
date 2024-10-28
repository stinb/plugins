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
  Test1 *t1;
  Test11 *t11 = dynamic_cast<Test11*>(t1); // UndCC_Violation
  
  Test2 *t2;
  Test22 *t22 = dynamic_cast<Test22*>(t2); // UndCC_Valid

  return 0;
}
