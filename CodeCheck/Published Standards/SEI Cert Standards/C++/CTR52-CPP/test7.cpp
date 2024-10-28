// Custom

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::fill_n(std::back_inserter(v), 3, -1); // UndCC_Valid
    for (int n : v)
        std::cout << n << ' ';
    std::cout << '\n';
}
