#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

class MutablePredicate : public std::unary_function<int, bool>
{
public:
    MutablePredicate() : numCalls(0) {}
    bool operator()(const int &) { return (++numCalls) == 3; }
private:
    size_t numCalls;
};


void My_Func(std::vector<int> v)
{

    int numCalls = 0;

    std::copy(v.begin(), std::remove_if(v.begin(), v.end(), [numCalls](const int &) mutable
                                        { return (++numCalls) == 3; }), // UndCC_Violation
              std::ostream_iterator<std::
                                        vector<int>::value_type>(std::cout, " "));
    std::cout << std::endl;
}

int main(void)
{
    std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    My_Func(v);

    return 0;
}
