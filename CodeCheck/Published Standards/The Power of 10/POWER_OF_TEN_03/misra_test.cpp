#include <cstdint>
#include <memory>
#include <vector>

auto i = std::make_unique<int32_t>(42); // UndCC_Violation(stdlib)
auto j = std::vector<int32_t>{};        // UndCC_Violation(stdlib)
