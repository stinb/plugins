
#include <cstdint>

typedef std::int32_t int1; // UNDCC_Valid
std::int32_t typedef int2; // UNDCC_Violation

class C
{
public:
  virtual inline void F1(); // UNDCC_Valid
  inline virtual void F2(); // UNDCC_Valid
  void virtual inline F3(); // UNDCC_Violation
private:
  std::int32_t mutable x; // UNDCC_Violation
  mutable std::int32_t y; // UNDCC_Valid
};
