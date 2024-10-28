// $Id: A12-6-1.cpp 271696 2017-03-23 09:23:09Z piotr.tanski $
#include <cstdint>
#include <string>

class A
{
public:
    A(std::int32_t n, std::string s) : number(n), str{s} // UNDCC_Valid
    {
    }
    // Implementation

private:
    std::int32_t number;
    std::string str;
};

class B
{
public:
    B(std::int32_t n, std::string s) // UNDCC_Violation - no member initializers
    {
        number = n;
        str = s;
    }
    // Implementation

private:
    std::int32_t number;
    std::string str;
};

class C
{
public:
    C(std::int32_t n, std::string s) : number{n}, str{s} // UNDCC_Valid
    {
        n += 1; // This does not violate the rule
        str.erase(str.begin(),
                  str.begin() + 1); // This does not violate the rule
    }
    // Implementation

private:
    std::int32_t number;
    std::string str;
};

class F
{
public:
    F(std::int32_t n, std::string s) : number{n} // UNDCC_Violation
    {
        str = s;
    }
    // Implementation

private:
    std::int32_t number;
    std::string str;
};

class G
{
public:
    G(std::int32_t n, std::string s) : str{s} // UNDCC_Violation
    {
        number = n;
    }
    // Implementation

private:
    std::int32_t number;
    std::string str;
};
