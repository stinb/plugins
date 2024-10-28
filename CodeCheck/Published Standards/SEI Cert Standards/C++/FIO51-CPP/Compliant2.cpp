#include <exception>
#include <fstream>
#include <string>

void f(const std::string &fileName)
{
    std::fstream file2(fileName);
    {
        std::fstream file(fileName);
        if (!file.is_open())
        {
            // Handle error
            return;
        }
    } // file is closed properly here when it is destroyed
    std::terminate();
}