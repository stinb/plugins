#include <cstdlib>

int main()
{
    std::system("ls -l >test.txt"); // UndCC_Violation
}