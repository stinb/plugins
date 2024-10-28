#include <cstdint>

// MISRA 2008

class A
{
public:
  virtual void withunusedpara    ( uint16_t * para1, // UndCC_Violation
                                   int16_t    unusedpara ) = 0;
  virtual void withoutunusedpara ( uint16_t * para1,
                                   int16_t  & para2      ) = 0;
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
