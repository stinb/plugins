#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

template <typename Iter>
void print_container(Iter B, Iter E)
{
    std::cout << "Contains: ";
    std::copy(B, E, std::ostream_iterator<decltype(*B)>(std::cout, " "));
    std::cout << std::endl;
}

void f()
{
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    print_container(v.begin(), v.end());
    v.erase(v.begin() + 3);
    print_container(v.begin(), v.end());
}