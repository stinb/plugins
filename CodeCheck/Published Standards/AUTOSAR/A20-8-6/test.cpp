#include <memory>
#include <vector>

void f(const std::vector<std::shared_ptr<int>> &shared_ptrs)
{
  for (const auto &shared_ptr : shared_ptrs) // UndCC_Valid (reference)
    ;

  for (const auto shared_ptr : shared_ptrs) // UndCC_Violation (constructed a copy)
    ;
}
