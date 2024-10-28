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

    file << "Output some data";

    std::string str;
    file.seekg(0, std::ios::beg);
    file >> str;  // UndCC_Valid
}
