// $Id: A8-4-3.cpp 308906 2018-02-23 15:34:15Z christof.meerwald $
#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <string>
#include <vector>

char * *pfile();

// A class that contains a memory resource.
class MemoryBlock
{
   // TODO: Add resources for the class here.
};

// UndCC_Valid: passing cheap-to-copy parameter by value
int32_t Increment(int32_t i)
{
  return i + 1;
}

// UndCC_Valid: passing expensive to copy parameter by reference to const
int32_t Sum(const std::vector<int32_t> &v)
{
  return std::accumulate(v.begin(), v.end(), 0);
}

// UndCC_Valid: passing in-out parameter by reference
void Decrement(int32_t &i)
{
  --i;
}

// UndCC_Valid: returning out parameter by value
std::string GetGreeting()
{
  return "Hello";
}

struct A
{
  std::string text;
  std::array<std::string, 1000> arr;
};

// Expensive to move "out" parameter passed by reference. If
// intentional, violation of A8-4-8 needs to be explained
void InitArray(std::array<std::string, 1000> &arr,
      const std::string &text)
{
  std::for_each(arr.begin(), arr.end(), [&text] (std::string &s) {
    s = text;
  });
}

// UndCC_Valid: passing in-out parameter by reference
void PopulateA(A &a)
{
  InitArray(a.arr, a.text);
}

// UndCC_Valid:
void f(MemoryBlock &&mem)
{
  // ..
}

// UndCC_Valid:
template <typename T>
T add(T &&a)
{
  // ...
}

// UndCC_Valid: skip pointers
void f3(char *c)
{
   // .. 
}


int32_t Sum2(const std::vector<int32_t> v) // UndCC_Violation
{
  return std::accumulate(v.begin(), v.end(), 0);
}
