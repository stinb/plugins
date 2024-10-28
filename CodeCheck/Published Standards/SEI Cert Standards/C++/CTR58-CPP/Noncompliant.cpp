#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

class MutablePredicate : public std::unary_function<int, bool>
{
    size_t timesCalled;

public:
    MutablePredicate() : timesCalled(0) {}

    bool operator()(const int &)
    {
        return ++timesCalled == 3;
    }
};

template <typename Iter>
void print_container(Iter b, Iter e)
{
    std::cout << "Contains: ";
    std::copy(b, e, std::ostream_iterator<decltype(*b)>(std::cout, " "));
    std::cout << std::endl;
}

void f()
{
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_container(v.begin(), v.end());

    v.erase(std::remove_if(v.begin(), v.end(), MutablePredicate()), v.end()); // UndCC_Violation
    print_container(v.begin(), v.end());
}
