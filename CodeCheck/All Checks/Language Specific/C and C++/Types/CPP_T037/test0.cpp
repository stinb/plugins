// AUTOSAR C++ 2014

#include <cstdint>
#include <numeric>
#include <string>
#include <vector>


int32_t Sum(std::vector<int32_t> &v) // UndCC_Violation - does not modify the "in-out" parameter
{
  return std::accumulate(v.begin(), v.end(), 0);
}


void AppendNewline(std::string &s) // UndCC_Valid - Modifying "in-out" parameter
{
  s += '\n';
}


void GetFileExtension(std::string &ext) // UndCC_Violation - Replacing parameter value
{
  ext = ".cpp";
}
