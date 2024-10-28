// M5-14-1.cpp
#include <cstdint>

#define QUADRAT(x) ((x) * (x))
#define HIGHEST(x, y) (((x) > 3 && (y) < 3) ? 0 : 1)

int f (int val);
bool f3 (int val);

int f2 (int val)
{
  val += 1;
  return val;
}

void func1()
{
  bool ishigh = true;
  int x=0, i=0;
  int32_t y = 1;
  int32_t yArr[5] = { 1, 2, 3, 4, 5 };
  
  if ( ishigh && ( x == i++ ) ) // UndCC_Violation
  { }
  if ( ishigh && ( x == f( x ) ) )  // Only acceptable if f(x) is
                                    // known to have no side effects
                                    // Complaint
  { }
  if ( ishigh || ( x == f2( x ) ) ) // UndCC_Violation
  { }
  
  if ( ishigh || ( x >= QUADRAT( ++x ) ) ) 
  { }
  
  bool sample = x >= 0 || x < 1;
  
  for (i = 0; i < 10 ; i++) {
    // .. 
  }
  
  for(int j = 0; j < 5; j++)
  {
    if (ishigh && (yArr[y++] != 4)) 
    {
      // ..
    }
  }
  
  for (int k = 0; ishigh || y <= 1; k++)
  {
      // ..
  }
  
}

int f (int val)
{
  return val ;
}
