#include <cstdlib>
#include <string>

void f()
{
    const char *tmpPtrVal = std::getenv("TMP");
    std::string tmp(tmpPtrVal ? tmpPtrVal : "");
    if (!tmp.empty())
    {
        // ...
    }
}