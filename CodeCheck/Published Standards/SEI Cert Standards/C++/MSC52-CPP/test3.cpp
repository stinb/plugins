#include <cstdint>
#include <stdexcept>

static std::int32_t F4(std::int32_t x, std::int32_t y)
{
  if (x > 100 || y > 100)
  {
    throw std::logic_error("Logic Error");
  }

  if (y > x)
  {
    return (y - x);
  }
} // UndCC_Violation - no return/throw in after the last if-statement

static std::int32_t F5(std::int32_t x, std::int32_t y)
{
  if (x > 100 || y > 100)
  {
    throw std::logic_error("Logic Error");
  }

  if (y > x)
  {
    throw std::logic_error("Logic Error");
  }
  return 1;
} // UndCC_Valid - return-throw in all exits paths

static std::int32_t F6(std::int32_t x, std::int32_t y)
{
  if (x > 100 || y > 100)
  {
    throw std::logic_error("Logic Error");
  }

  if (y > x)
  {
    return (y - x);
  }
	else
	{
		return 0;
	}
} // UndCC_Valid - return-throw in all exits paths
