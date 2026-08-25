#include <cstdint>
namespace M1 {
struct vector { uint16_t x; uint16_t y; uint16_t z; } vector;  // UndCC_Violation
}
namespace M2 {
struct vec2 { uint16_t x; };
int vec2 = 0;                                                  // UndCC_Violation
}
namespace M3 {
struct ok { uint16_t x; };
ok okObj;                                                      // UndCC_Valid
}
