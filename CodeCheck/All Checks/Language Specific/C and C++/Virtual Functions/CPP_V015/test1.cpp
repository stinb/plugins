typedef unsigned short uint16_t;
typedef signed short int16_t;

class A
{
public:
  virtual void withunusedpara    ( uint16_t * para1, // UndCC_Violation
								   int16_t ) = 0;
  virtual void withoutunusedpara ( uint16_t * para1,
								   int16_t  & para2 ) = 0;
};

class C
{
public:
	virtual void unused ( uint16_t para1, // UndCC_Violation
						  uint16_t unused1)
	{
	}
	virtual void used ( uint16_t para1,
						uint16_t)
	{
	}
};

class D1: public C
{
public:
	virtual void unused ( uint16_t para1,
						  uint16_t para2)
	{
		para1 = 1;
	}
	virtual void used (uint16_t para1,
					   uint16_t para2)
	{
		para1 = 2;
		para2 = 3;
	}
};

class B1: public A
{
public:
  virtual void withunusedpara ( uint16_t * para1,
								int16_t    unusedpara )
  {
    *para1 = 1U;
  }
virtual void withoutunusedpara ( uint16_t * para1,
								 int16_t  & para2 )
  {
    *para1 = 1U;
  }
};
class B2: public A
{
public:
  virtual void withunusedpara ( uint16_t * para1,
								int16_t    unusedpara )
  {
    *para1 = 1U;
  }
  virtual void withoutunusedpara ( uint16_t * para1,
								   int16_t  & para2 )
  {
    para2 = 0;
  }
};
