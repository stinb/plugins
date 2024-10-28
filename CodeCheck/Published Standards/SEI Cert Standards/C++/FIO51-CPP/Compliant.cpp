#include <exception>
#include <fstream>
#include <string>

void f(const std::string &fileName)
{
    std::fstream file(fileName);
    if (!file.is_open())
    {
        // Handle error
        return;
    }
    // ...
    file.close();
    if (file.fail())
    {
        // Handle error
    }
    std::terminate();
}