#include <fstream>
#include <string>

void f(std::istream &in)
{
    char buffer[32];
    try
    {
        in.read(buffer, sizeof(buffer));
    }
    catch (std::ios_base::failure &e)
    {
        // Handle error
    }
    std::string str(buffer, in.gcount());
    // ...
}