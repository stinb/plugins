// $Id: A8-2-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>

template <typename T>

class A
{
public:
    using Type = std::int32_t;

    Type F(T const &) noexcept;
    Type G(T const &) noexcept;
};

template <typename T>
typename A<T>::Type A<T>::F(T const &) noexcept // UndCC_Violation
{
    // Implementation
}

template <typename T>
auto A<T>::G(T const &) noexcept -> Type // UndCC_Valid
{
    // Implementation
}