#include <exception>
#include <fstream>
#include <string>

void f(const std::string &fileName)
{
    std::fstream file(fileName);
    if (!file.is_open())        // UndCC_Violation
    {
        // Handle error
        return;
    }
    // ...
    std::terminate();
}
