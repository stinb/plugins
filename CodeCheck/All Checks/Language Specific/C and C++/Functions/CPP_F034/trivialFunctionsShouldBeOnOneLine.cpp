//% $Id: A3-1-6.cpp 305629 2018-01-29 13:29:25Z piotr.serwa $
#include <cstdint>

class A
{
  public:
  A(std::int32_t l) noexcept : limit{l} {}
  // UndCC_Valid
  std::int32_t Limit() const noexcept { return limit; }
  // UndCC_Valid
  void SetLimit(std::int32_t l) { limit = l; }
	//not getter not setter
  std::int32_t DivideLimit(std::int32_t l) { return limit/l; }
	//not getter not setter
	std::int32_t AddToLimit(std::int32_t l) 
	{ 
		std::int32_t sum = l + limit;
		return sum; 
	}
	//non-compliant, not inline
	std::int32_t LimitMinimum() const noexcept  // UndCC_Violation
	{ 
		return limitMin; 
	}
	// UndCC_Valid
	void SetMinLimit(std::int32_t l) { limitMin = l; }
  //non-compliant
  //std::int32_t Limit() const noexcept
  //{
    //open file, read data, close file
    //return value
  //}
  
  //non-compliant
  //void SetLimit(std::int32_t l)
  //{
    //open file, write data, close file
  //}
  
  private:
  std::int32_t limit;
	std::int32_t limitMin;
};
