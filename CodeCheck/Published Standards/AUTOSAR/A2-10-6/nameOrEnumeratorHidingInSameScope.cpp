//% $Id: A2-10-6.cpp 313821 2018-03-27 11:16:14Z michal.szczepankiewicz $
#include <cstdint>

namespace NS1
{
    class G
    {
    };
    void G() {} // UndCC_Violation hides class G
}

namespace NS2
{
    enum class H
    {
        VALUE = 0,
    };
    std::uint8_t H = 17; // UndCC_Violation, hides
    //scoped enum H
}

namespace NS3
{
    class J
    {
    };
    enum H //does not hide NS2::H, but non-compliant to A7-2-3
    {
        J = 0, /// UndCC_Violation, hides class J
    };
}

namespace NS4
{
    enum K //does not hide NS2::H, but non-compliant to A7-2-3
    {
        X = 0,
    };
    void foo()
    {
        int K;  
    }
}

namespace NS5
{
    enum K //does not hide NS2::H, but non-compliant to A7-2-3
    {
        X = 0,
    };
    void foo()
    {
        enum A //does not hide NS2::H, but non-compliant to A7-2-3
        {
            X = 0,
        };
        int A; // UndCC_Violation
    }
}

int main(void)
{
    NS1::G();
    //NS1::G a; //compilation error, NS1::G is a function
    //after a name lookup procedure
    class NS1::G a
    {
    }; //accessing hidden class type name

    enum NS2::H b; //accessing scoped enum NS2::H
    NS2::H = 7;

    class NS3::J c
    {
    }; //accessing hidden class type name
    std::uint8_t z = NS3::J;
}
