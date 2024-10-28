// $Id: A14-7-2.cpp 312645 2018-03-21 11:44:35Z michal.szczepankiewicz $
#include <cstdint>
#include <vector>
#include "traits.hpp"

//non-compliant, not declared
//in the same file as
//is_serializable class
template <>
struct is_serializable<std::uint16_t> : std::true_type {}; // UndCC_Violation

template <typename T, typename = std::enable_if<is_serializable<T>::value>>
std::vector<std::uint8_t> serialize(const T& t)
{
    //only a basic stub
    return std::vector<std::uint8_t>{t};
}

#include <string>
int main() {
    serialize(std::uint8_t{3});
}
