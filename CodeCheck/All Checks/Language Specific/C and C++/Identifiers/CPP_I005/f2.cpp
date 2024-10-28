#include <cstdint>

// f2.cpp
namespace ns1
{
    static std::int32_t globalvariable = 0; // UndCC_Violation - identifier reused in ns1 namespace in f1.cpp
}
namespace ns2
{
    static std::int32_t globalvariable = 0; // UndCC_Valid - identifier reused, but in another namespace
}
