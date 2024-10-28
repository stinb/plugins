// $Id: A8-4-8.cpp 306164 2018-02-01 15:04:53Z christof.meerwald $

#include <iostream>
#include <vector>
// UndCC_Valid, return value
std::vector<int> SortOutOfPlace(const std::vector<int> &inVec);


void FindAll(const std::vector<int> &inVec, std::vector<int> &outVec); // UndCC_Violation

struct B
{
};

struct BB
{
    B GetB() const & { return obj; }
    B &&GetB() && { return std::move(obj); }

    B obj;
};


BB &&MakeBb1() // UndCC_Violation
{
    return std::move(BB());
}

// UndCC_Valid: uses compiler copy-ellision
BB MakeBb2()
{
    return BB();
}

int main()
{
    BB x = MakeBb2();

    auto cpd = x.GetB();         // copied value
    auto mvd = MakeBb2().GetB(); // moved value

    return 0;
}
