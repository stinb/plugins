// $Id: A7-1-6.cpp 271687 2017-03-23 08:57:35Z piotr.tanski $
#include <cstdint>
#include <type_traits>

typedef std::int32_t (*fPointer1)(std::int32_t); // UndCC_Violation

using fPointer2 = std::int32_t (*)(std::int32_t); // UndCC_Valid

// template <typename T>


template <typename T>
using fPointer3 = std::int32_t (*)(T); // UndCC_Valid
