#include <iostream>
#include <type_traits>
#include <utility>

template <typename T>
class is_incrementable
{
    typedef char one[1];
    typedef char two[2];
    static one &is_incrementable_helper(decltype(std::declval<typename std::remove_cv<T>::type &>()++) *p);
    static two &is_incrementable_helper(...);

public:
    static const bool value = sizeof(is_incrementable_helper(nullptr)) == sizeof(one);
};

void f()
{
    std::cout << std::boolalpha << is_incrementable<int>::value;
}