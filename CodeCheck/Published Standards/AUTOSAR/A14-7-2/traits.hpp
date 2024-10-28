#include <type_traits>
#include <cstdint>
#include <iostream>

// primary template
template <typename T>
struct is_serializable : std::false_type {};

// UndCC_Valid, case (1)
template <>
struct is_serializable<std::uint8_t> : std::true_type {};

template <class T>
void sort(T arr[], int size)
{
    // code to implement Quick Sort
}

template <class T>
class Test
{
    // Data members of test
public:
    Test()
    {
        // Initialization of data members
        std::cout << "General template object \n";
    }
    // Other methods of Test
};
