#include <cstdint>

int x = 0; // UndCC_Violation - use of int
int32_t y = 0;            // UndCC_Valid
int_least32_t z = 0;      // UndCC_Valid
using torque_t = int;     // UndCC_Valid by exception #1
torque_t w = 0;

class C
{
public:
    C operator++(int); // UndCC_Valid by exception #2.1
};

int main() {} // UndCC_Valid by exception #2.2
// int main(int argc, char *argv[]) {} // UndCC_Valid by exception #2.2 and #2.3
