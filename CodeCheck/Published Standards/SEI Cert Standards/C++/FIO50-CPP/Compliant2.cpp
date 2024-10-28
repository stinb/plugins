#include <fstream>
#include <string>
#include <cstdio>

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
    file.seekp(1);
    file >> str; // UndCC_Valid
}
