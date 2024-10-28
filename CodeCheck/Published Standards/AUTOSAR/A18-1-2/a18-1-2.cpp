// $Id: A18-1-2.cpp 312108 2018-03-16 17:56:49Z jan.babst $
#include <cstdint>
#include <vector>
                                                                          
class BoolWrapper
{
public:
  BoolWrapper() = default;
  constexpr BoolWrapper(bool b) : b_(b) {} // implicit by design
  constexpr operator bool() const { return b_; } // implicit by design
  private:
  bool b_{};
};
                                                                          
void Fn() noexcept
{
  std::vector<std::uint8_t> v1; // UndCC_Valid
  std::vector<bool> v2; // UndCC_Violation
  std::vector<BoolWrapper> v3{true, false, true, false}; // UndCC_Valid
}
