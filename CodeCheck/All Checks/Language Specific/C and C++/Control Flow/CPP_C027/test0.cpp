class C // UndCC_Violation - b and c
{
public:
  int b;
protected:
  int c;
private:
  int d;
};

class POD
{
public:
  int a;
};

class nonPOD
{
private:
  int e;
};
